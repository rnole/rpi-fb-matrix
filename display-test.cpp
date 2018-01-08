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

#include "Config.h"
#include "glcdfont.h"
#include "GridTransformer.h"
#include "utils.h"

using namespace std;
using namespace rgb_matrix;


// Global to keep track of if the program should run.
// Will be set false by a SIGINT handler when ctrl-c is
// pressed, then the main loop will cleanly exit.
volatile bool running = true;



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

    stringstream pos;
    pos << "Temperatura:32" << '\xF8' << "C";
    printCanvas(canvas, 0, 0, pos.str());

    /*
    for (int j=0; j<panel_rows; ++j) {
    	for (int i=0; i<panel_columns; ++i) {
        	// Compute panel origin position.
        	int x = i*config.getPanelWidth();
        	int y = j*config.getPanelHeight();
        	// Print the current grid position to the top left (origin) of the panel.
        	stringstream pos;
        	pos << i << "," << j;
       		printCanvas(canvas, x, y, pos.str());
      }
    }
    */


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
