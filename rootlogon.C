#include <TStyle.h>

#include <iostream>

void rootlogon() {
    gStyle->SetOptStat(0);
    gStyle->SetTitleFontSize(0.07);
    gStyle->SetPadTopMargin(0.1);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadBottomMargin(0.10);
    gStyle->SetPadLeftMargin(0.12);
}
