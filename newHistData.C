#include <TAxis.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TString.h>
#include <TSystem.h>
#include <include/AnnualData.h>

#include <iostream>
#include <sstream>
#include <vector>
void newHistData() {

    // Load shared object
    gSystem->Load("AnnualData.so");

    // Get vectors using the class directly
    AnnualData ad("datasets/Boras_Data_Cleaned.csv", "18:00:00");
    std::vector<Int_t> years = ad.years();
    std::vector<Int_t> counts = ad.count(16.6, 18.1);
    std::vector<Int_t> ranges = ad.range(16.6, 18.1);
    std::vector<Int_t> amplitudes = ad.amplitude(1.5);





    // Check that all data vectors are the same size
    if (years.size() != counts.size() || years.size() != ranges.size() || years.size() != amplitudes.size()) {
        std::cerr << "Error: Data vectors have mismatched sizes!" << std::endl;
        return;
    }

    // Determine histogram range based on the years vector
    Int_t yearMin = *std::min_element(years.begin(), years.end());
    Int_t yearMax = *std::max_element(years.begin(), years.end());
    Int_t nBins = years.size();

    // Create a canvas for the histograms
    TCanvas *c1 = new TCanvas("c1", "Yearly Data Histograms", 1200, 900);
    //c1->Divide(2, 2);

  
    auto calcMeanStdDev = [](const std::vector<Int_t> &values){
        Double_t sum = 0.0, sumSq = 0.0;
        for (auto val : values) {
            sum += val;
            sumSq += val * val;
        }
        Double_t mean = sum / values.size();
        Double_t variance = (sumSq / values.size()) - (mean * mean);
        return std::make_pair(mean, std::sqrt(variance));
    };

    // Counts histogram
    c1->cd();
    TH1F *hCounts = new TH1F("hCounts", "Counts over Years;Year;Counts", nBins, years.front() - 0.5, years.back() + 0.5);
    for (size_t i = 0; i < years.size(); i++) {
        Int_t bin = hCounts->FindBin(years[i]);
        hCounts->SetBinContent(bin, counts[i]);
    }
    hCounts->SetFillColor(kGreen-9);
    hCounts->Draw();

    TF1 *qFit = new TF1("qFit", "pol3", years.front(), years.back());
    hCounts->Fit(qFit, "R");
    qFit->SetLineColor(kRed);
    qFit->Draw("same");
/*
    Double_t chi2_counts = qFit->GetChisquare();
    Int_t ndf_counts = qFit->GetNDF();
    TLatex latex;
    latex.SetTextSize(0.03);
    latex.DrawLatexNDC(0.75, 0.65, Form("#frac{#chi^{2}}{ndf} = %.3f", chi2_counts / ndf_counts));
*/

    auto [meanCounts, stdDevCounts] = calcMeanStdDev(counts);
    TLatex stats1;
    stats1.SetNDC();
    stats1.SetTextSize(0.03);
    stats1.DrawLatex(0.75, 0.90, Form("Mean = %.2f", meanCounts));
    stats1.DrawLatex(0.75, 0.85, Form("Std Dev = %.2f", stdDevCounts));
    c1->Draw();
    c1->Print("ROOT/c1.tex");
    c1->Print("ROOT/c1.png");

    // Ranges histogram
    TCanvas *c2 = new TCanvas("c2", "Yearly Data Histograms", 1200, 900);
    c2->cd();

    TH1F *hRanges = new TH1F("hRanges", "Ranges over Years;Year;Ranges", nBins, yearMin - 0.5, yearMax + 0.5);
    for (size_t i = 0; i < years.size(); i++) {
        Int_t bin = hRanges->FindBin(years[i]);
        hRanges->SetBinContent(bin, ranges[i]);
    }
    hRanges->SetFillColor(kGreen-9);
    hRanges->Draw();

    TF1 *qFit2 = new TF1("qFit2", "pol3", yearMin, yearMax);
    hRanges->Fit(qFit2, "Q");
    qFit2->SetLineColor(kRed);
    qFit2->Draw("same");

    auto [meanRanges, stdDevRanges] = calcMeanStdDev(ranges);
    TLatex stats2;
    stats2.SetNDC();
    stats2.SetTextSize(0.03);
    stats2.DrawLatex(0.725, 0.90, Form("Mean = %.2f", meanRanges));
    stats2.DrawLatex(0.725, 0.85, Form("Std Dev = %.2f", stdDevRanges));
    // Amplitudes histogram
    c2->Draw();
    c2->Print("ROOT/c2.tex");
    c2->Print("ROOT/c2.png");
    TCanvas *c3 = new TCanvas("c3", "Yearly Data Histograms", 1200, 900);
    c3->cd();

    TH1F *hAmplitudes = new TH1F("hAmplitudes", "Amplitudes over Years;Year;Amplitudes", nBins, yearMin - 0.5, yearMax + 0.5);
    for (size_t i = 0; i < years.size(); i++) {
        Int_t bin = hAmplitudes->FindBin(years[i]);
        hAmplitudes->SetBinContent(bin, amplitudes[i]);
    }
    hAmplitudes->SetFillColor(kGreen-9);
    hAmplitudes->Draw();

    TF1 *qFit3 = new TF1("qFit2", "pol3", yearMin, yearMax);
    hAmplitudes->Fit(qFit3, "Q");
    qFit3->SetLineColor(kRed);
    qFit3->Draw("same");

    auto [meanAmplitudes, stdDevAmplitudes] = calcMeanStdDev(amplitudes);
    TLatex stats3;
    stats3.SetNDC();
    stats3.SetTextSize(0.03);
    stats3.DrawLatex(0.75, 0.90, Form("Mean = %.2f", meanAmplitudes));
    stats3.DrawLatex(0.75, 0.85, Form("Std Dev = %.2f", stdDevAmplitudes));

    c3->Draw();

    c3->Print("ROOT/c3.tex");
    c3->Print("ROOT/c3.png");
   // f1->Close();

}
