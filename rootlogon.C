
#include <TStyle.h>

#include <iostream>
void rootlogon() {
  // Add any code here you want to run automatically when you launch ROOT. Here
  // are some example style settings you can use!
  /*std::cout << "This output comes from the rootlogon.C macro which ROOT will "
               "run automatically on startup!"
            << std::endl;*/
  gStyle->SetOptStat(
      0);  // Let's make our plots look a bit better. Get rid of the stat box
  /*gStyle->SetOptTitle(
      0); */ // Get rid of the title (good plots use legends instead)
  gStyle->SetTitleFontSize(0.065);
  gStyle->SetTitleSize(0.04, "x");

  gStyle->SetTitleSize(0.04, "y");
  gStyle->SetLabelSize(0.04, "x");  // Use bigger labels too
  gStyle->SetLabelSize(0.04, "y");
  gStyle->SetPadTopMargin(0.1);  // Change the margins to fit our new sizes
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.10);
  gStyle->SetPadLeftMargin(0.10);
}
