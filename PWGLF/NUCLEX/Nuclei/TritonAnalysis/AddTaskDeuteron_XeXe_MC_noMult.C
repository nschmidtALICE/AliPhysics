#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Rtypes.h>
#include <TString.h>
#include "AliAnalysisManager.h"
#include "AliAnalysisDataContainer.h"
#include "AliAnalysisTaskDeuteron_XeXe_MC_noMult.h"
#include "AliPID.h"
#endif

//____________________________________________________________________________________
AliAnalysisTask *AddTaskDeuteron_XeXe_MC_noMult(unsigned long fTriggerMask, Double_t CentralityMin, Double_t CentralityMax, Double_t VertexZmin, Double_t VertexZmax, Int_t NumberOfVertexContributorsMin, const char *CentralityEstimator, Double_t PtMin, Double_t PtMax, Double_t EtaMax, Double_t YMax, Int_t NumberClustersITSMin, Int_t NumberClustersTPCMin, Int_t NumberCrossedRowsTPCMin, Double_t CrossedRowsFindableClsMin, Int_t NumberClustersTPCdEdxMin, Double_t ChiSquarePerNDFMax, const char *ITSrequirement, Double_t DCAzMax, Double_t DCAxyMax, Double_t nSigmaTOFmax, Double_t nSigmaTPCmax, Int_t TRDntracklets, Double_t par0_mean_TPC, Double_t par1_mean_TPC, Double_t par0_sigma_TPC, Double_t par0_mean_TOF, Double_t par1_mean_TOF, Double_t par0_sigma_TOF, Double_t par1_sigma_TOF )  {

  //Get the current analysis manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    Error("AddTaskDeuteron_XeXe_MC_noMult","No analysis manager found.");
    return 0;
  }

  // Check the analysis type using the event handlers connected to the analysis manager.
  //==============================================================================
  if (!mgr->GetInputEventHandler()) {
    ::Error("AddTaskDeuteron_XeXe_MC_noMult", "This task requires an input event handler");
    return NULL;
  }

    //Input container
    AliAnalysisDataContainer *input = mgr -> GetCommonInputContainer();

    //Analysis Task
    AliAnalysisTaskDeuteron_XeXe_MC_noMult *task = new AliAnalysisTaskDeuteron_XeXe_MC_noMult ("task");
    task -> SelectCollisionCandidates (fTriggerMask);
    task -> SetCentralityRange        (CentralityMin,CentralityMax);
    task -> SetEventCuts              (VertexZmin,VertexZmax,NumberOfVertexContributorsMin,CentralityEstimator);
    task -> SetTrackCuts              (PtMin, PtMax, EtaMax, YMax, NumberClustersITSMin,NumberClustersTPCMin, NumberCrossedRowsTPCMin,CrossedRowsFindableClsMin, NumberClustersTPCdEdxMin, ChiSquarePerNDFMax, ITSrequirement,DCAzMax, DCAxyMax, nSigmaTOFmax, nSigmaTPCmax, TRDntracklets);
    task -> RecalibrationTPCandTOF (par0_mean_TPC, par1_mean_TPC, par0_sigma_TPC, par0_mean_TOF, par1_mean_TOF, par0_sigma_TOF, par1_sigma_TOF );
    //task -> SelectCollisionCandidates (AliVEvent::kINT7);

    mgr -> AddTask(task);

    AliAnalysisDataContainer *outputList_MC = mgr -> CreateContainer("outputList_MC", TList::Class(), AliAnalysisManager::kOutputContainer, "AnalysisResults.root");
    AliAnalysisDataContainer *outputQA      = mgr -> CreateContainer("outputQA",      TList::Class(), AliAnalysisManager::kOutputContainer, "AnalysisResults.root");
    //    AliAnalysisDataContainer *outputQA   = mgr -> CreateContainer("InputQA",TList::Class(),AliAnalysisManager::kOutputContainer,"QA.root");
    mgr -> ConnectInput (task,0,input);
    mgr -> ConnectOutput(task,1,outputList_MC);
    mgr -> ConnectOutput(task,2,outputQA);



    return task;

}
