// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Program to aid in the testing of LED matrix chains.
// Author: Tony DiCola
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <led-matrix.h>
#include <signal.h>
#include <unistd.h>
#include <json/json.h>

#include "Config.h"
#include "glcdfont.h"
#include "GridTransformer.h"
#include "utils.h"
#include "libcurl.h"

using namespace std;
using namespace rgb_matrix;


// Global to keep track of if the program should run.
// Will be set false by a SIGINT handler when ctrl-c is
// pressed, then the main loop will cleanly exit.
volatile bool running = true;

static int temperature_value = 0;
static int humidity_value = 0;
static int noise_value = 0;


void json_parse(json_object *jobj){

	json_object *jarray = jobj;
	json_object *jvalue;
	json_object *jval_temperature;
	json_object *jval_humidity;
	json_object *jval_noise;
		
	json_object_object_get_ex(jobj, "message", &jarray);

	int arraylen = json_object_array_length(jarray);
	printf("Array length: %d\n", arraylen);
	
	jvalue = json_object_array_get_idx(jarray, 7);
	json_object_object_get_ex(jvalue, "temp", &jval_temperature);
	
	jvalue = json_object_array_get_idx(jarray, 8);
	json_object_object_get_ex(jvalue, "hum", &jval_humidity);
	
	jvalue = json_object_array_get_idx(jarray, 14);
	json_object_object_get_ex(jvalue, "sonido", &jval_noise);

	temperature_value = json_object_get_int(jval_temperature);
	humidity_value    = json_object_get_int(jval_humidity);
	noise_value       = json_object_get_int(jval_noise);

		
	printf("Noise: %d\n", json_object_get_int(jval_noise));
	printf("Temperature: %d\n", json_object_get_int(jval_temperature));
	printf("Humidity: %d\n", json_object_get_int(jval_humidity));
	
}



void printCanvas(Canvas* canvas, int x, int y, const string& message,
                 int r = 255, int g = 0, int b = 0) {
  // Loop through all the characters and print them starting at the provided
  // coordinates.
 int array_chars[256] = {0};
 
 for(int pos=0;  pos< message.length(); pos++){
	 for(int i=1; i <= 5; i++)
		array_chars[pos*6 +i] = glcdfont[message.at(pos)*5 + i-1];
 }

 
 for(int loop=0; loop< message.length()*6 +1; loop++){
	 x = 0;

	 for(int i=loop; i < loop +64; i++){

		for(int j=0; j <8; j++){
			if((array_chars[i] >>j) & 0x01)
				Transformer(canvas, x, y+j, 0xFF, 0, 0);
			else
				Transformer(canvas, x, y+j, 0, 0, 0);
		}
		x += 1;
	 }
	 usleep(100000);
 }

}

static void sigintHandler(int s) {
  running = false;
}

static void usage(const char* progname) {
  std::cerr << "Usage: " << progname << " [flags] [config-file]" << std::endl;
  std::cerr << "Flags:" << std::endl;
  rgb_matrix::PrintMatrixFlags(stderr);
}

int main(int argc, char** argv) {
  try {
    // Initialize from flags.
    rgb_matrix::RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_options;
    if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv,
                                           &matrix_options, &runtime_options)) {
      usage(argv[0]);
      return 1;
    }

    Config config(&matrix_options, argc >= 2 ? argv[1] : "/dev/null");
    cout << "Using config values: " << endl
         << " display_width: " << config.getDisplayWidth() << endl
         << " display_height: " << config.getDisplayHeight() << endl
         << " panel_width: " << config.getPanelWidth() << endl
         << " panel_height: " << config.getPanelHeight() << endl
         << " chain_length: " << config.getChainLength() << endl
         << " parallel_count: " << config.getParallelCount() << endl;

    // Initialize matrix library.
    // Create canvas and apply GridTransformer.
    RGBMatrix *canvas = CreateMatrixFromOptions(matrix_options, runtime_options);

    int panel_rows = config.getParallelCount();
    int panel_columns = config.getChainLength();
    if (config.hasTransformer()) {
      GridTransformer grid = config.getGridTransformer();
      canvas->ApplyStaticTransformer(grid);
      panel_rows = grid.getRows();
      panel_columns = grid.getColumns();
    }

    cout << " grid rows: " << panel_rows << endl
         << " grid cols: " << panel_columns << endl;

    // Clear the canvas, then draw on each panel.
    canvas->Fill(0, 0, 0);

    CURL *conn = NULL;
    CURLcode code;
    curl_global_init(CURL_GLOBAL_DEFAULT);


    // Initialize CURL connection
    if(!init(conn, argv[2])) {
	fprintf(stderr, "Connection initializion failed\n");
	exit(EXIT_FAILURE);
    }


    // Retrieve content for the URL
    code = curl_easy_perform(conn);
    curl_easy_cleanup(conn);

    if(code != CURLE_OK) {
	fprintf(stderr, "Failed to get website\n");
	exit(EXIT_FAILURE);
    }

    char *buffer_str = (char *)buffer.c_str();	
    json_object *jobj= json_tokener_parse(buffer_str);
    json_parse(jobj);


    stringstream temperature;
    stringstream humidity;
    stringstream noise;
    temperature << "Temperatura:" << temperature_value << '\xF8' << "C";
    humidity	<< "Humedad:" << humidity_value  << "%";
    noise	<< "Ruido:" << noise_value  << "dB";
    printCanvas(canvas, 0, 0, temperature.str());
    printCanvas(canvas, 0, 10, humidity.str());
    printCanvas(canvas, 0, 20, noise.str());

    // Loop forever waiting for Ctrl-C signal to quit.
    signal(SIGINT, sigintHandler);
    cout << "Press Ctrl-C to quit..." << endl;
    while (running) {
      sleep(1);
    }
    canvas->Clear();
    delete canvas;
  }
  catch (const exception& ex) {
    cerr << ex.what() << endl;
    usage(argv[0]);
    return -1;
  }
  return 0;
}



