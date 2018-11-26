
void Draw_PC_Origin(const char* name)
{
  TFile* file = new TFile(name);

  TTree* t1 = new TTree();
  
  file->GetObject("PC",t1); // PC is tree form

  TCanvas* c1 = new TCanvas();
  c1->Divide(3,2);

  t1->SetMarkerStyle(20); // set marker type to black circle
  
  c1->cd(1);
  t1->Draw("OriginX:OriginY>>PC1_Origin(1600,-800,799, 1000,-500,499)", "PC1Edep != 0");
  TH2F* PC1_Origin = (TH2F*)gDirectory->Get("PC1_Origin");
  //PC1_Origin->SetBins(1600,-800,799, 1000,-500, 499);
  PC1_Origin->SetTitle("PC1_Origin");
  PC1_Origin->SetTitleOffset(2);
  PC1_Origin->Draw();

   c1->cd(2);
  t1->Draw("OriginX:OriginY>>PC2_Origin(1600,-800,799, 1000,-500,499)", "PC2Edep != 0");
  TH2F* PC2_Origin = (TH2F*)gDirectory->Get("PC2_Origin");
  //  PC2_Origin->SetBins(50,-900,700, 50,-500, 500);
  PC2_Origin->SetTitle("PC2_Origin");
  PC2_Origin->SetTitleOffset(2);
  PC2_Origin->Draw();

   c1->cd(3);
  t1->Draw("OriginX:OriginY>>PC3_Origin(1600,-800,799, 1000,-500,499)", "PC3Edep != 0");
  TH2F* PC3_Origin = (TH2F*)gDirectory->Get("PC3_Origin");
  //  PC3_Origin->SetBins(500,-1000,999, 300,-600, 599);
  PC3_Origin->SetTitle("PC3_Origin");
  PC3_Origin->SetTitleOffset(2);
  PC3_Origin->Draw();

   c1->cd(4);
  t1->Draw("OriginX:OriginY>>PC23_Origin(1600,-800,799, 1000,-500,499)", "PC2Edep != 0 && PC3Edep !=0");
  TH2F* PC23_Origin = (TH2F*)gDirectory->Get("PC23_Origin");
  //PC23_Origin->SetBins(1600,-800,799, 1000,-500, 499); 
  PC23_Origin->SetTitle("PC23_Origin");
  PC23_Origin->SetTitleOffset(2);
  PC23_Origin->Draw();

   c1->cd(5);
  t1->Draw("OriginX:OriginY>>PC123_Origin(1600,-800,799, 1000,-500,499)", "PC1Edep != 0 && PC2Edep != 0 && PC3Edep != 0");
  TH2F* PC123_Origin = (TH2F*)gDirectory->Get("PC123_Origin");
  // PC123_Origin->SetBins(1600,-800,799, 1000,-500, 499);
  PC123_Origin->SetTitle("PC123_Origin");
  PC123_Origin->SetTitleOffset(2);
  PC123_Origin->Draw();


  
}
  
