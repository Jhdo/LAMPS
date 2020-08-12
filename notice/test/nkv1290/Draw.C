void Draw(char* filename)
{
    TFile *fin = new TFile(filename);
    TTree *tree = (TTree*) fin->Get("tree_out");
    int ntdc = -999;
    double tdc[100] = {-999,};
    int tdc_ch[100] = {-999,};
    int triggerID = -999;
    int eventID = -999;
    tree->SetBranchAddress("ntdc", &ntdc);
    tree->SetBranchAddress("tdc", tdc, "tdc[100]/D");
    tree->SetBranchAddress("tdc_ch", tdc_ch, "tdc_ch[100]/I");
    tree->SetBranchAddress("triggerID", &triggerID);
    tree->SetBranchAddress("eventID", &eventID);

    TH1D *htdc_diff = new TH1D("htdc_diff", "htdc_diff", 400, -5, 5);

    int nent = tree->GetEntries();
    for (int ie = 0; ie < nent; ie++){
        tree->GetEntry(ie);
        if (ntdc != 2) continue;
        if (tdc_ch[0] == tdc_ch[1]) continue;
        htdc_diff->Fill(tdc[0] - tdc[1]);
    }

    htdc_diff->Draw();
    
    return;
}