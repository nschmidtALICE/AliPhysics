#ifndef ALIJHFTAGGING_H
#define ALIJHFTAGGING_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice     */

//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

#include "AliAnalysisTaskEmcalJet.h"

class TRandom3;
class AliEmcalJet;
class AliHFJetsTagging;
class AliVertexerTracks;
class AliFJWrapper;
class AliJetContainer;
class AliHFJetsTaggingVertex;
class AliRDHFJetsCutsVertex;
//==============================================================

class AliJHFTagging : public AliAnalysisTaskEmcalJet
{
public:
  enum TTypeImpPar
  {
    kXY,
    kXYSig,
    kXYZ,
    kXYZSig,
    kXYZSigmaOnly
  };

  AliJHFTagging();
  AliJHFTagging(const char *name);
  AliJHFTagging(const AliJHFTagging &ap);
  AliJHFTagging &operator=(const AliJHFTagging &ap);
  virtual ~AliJHFTagging();

  // methods to fill from AliAnalysisTaskSE.
  virtual void UserCreateOutputObjects();
  virtual void Init();
  virtual void LocalInit() { Init(); }
  //   virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *opt = "");
  virtual bool Run();

  TClonesArray *GetInputList() const { return fInputList; }     //! This is for both data and MC
  TClonesArray *GetInputListb() const { return fInputListb; }   //! This is for MC
  TClonesArray *GetInputListc() const { return fInputListc; }   //! This is for MC
  TClonesArray *GetInputListlf() const { return fInputListlf; } //! This is for MC

  double GetDeltaPtRandomCone();
  double GetDeltaPtPerpEmbedding(double signalEta, double signalPhi);
  void SetEmbeddPerpendicular(bool EmbeddPerpendicular = true) { fEmbeddPerpendicular = EmbeddPerpendicular; }; // EMB_clus

  void DoJetProbabilityAnalysis(bool val = true) { fDoJetProbabilityAnalysis = val; }
  void UsePartonDefinition(bool val = true) { fUsePartonDef = val; }

  void FillCandidateJet(int CutIndex, int JetFlavor);
  void FillCandidateHFJet(AliEmcalJet *jet, TClonesArray *inputList);
  void FillCandidateHFJetMC(AliEmcalJet *jet, short jetFlavor);

  bool SetResFunction(TF1 *f, int j)
  {
    fResolutionFunction[j] = f;
    return true;
  }
  bool SetResFunctionb(TF1 *f, int j)
  {
    fResolutionFunctionb[j] = f;
    return true;
  }
  bool SetResFunctionc(TF1 *f, int j)
  {
    fResolutionFunctionc[j] = f;
    return true;
  }
  bool SetResFunctionlf(TF1 *f, int j)
  {
    fResolutionFunctionlf[j] = f;
    return true;
  }

  void SetMinTrackProbability(double value) { fMinTrackProb = value; }

  void SetDoSVAnalysis(bool value) { fDoSVAnalysis = value; }
  void SetDoTCAnalysis(bool value) { fDoTrackCountingAnalysis = value; }
  void SetDo3DSVHistos(bool value) { fDo3DSVHists = value; }

  void SetTaggerWorkingPoint(double value) { fThresholdIP = value; }
  void SetMaxDispersion(double value) { fMaxDespersion = value; }
  void SetThresholdLxy(double value) { fThresholdLxy = value; }

  // B jet tracks selection
  void SetTrackMinPt(double val) { fTCMinTrackPt = val; }
  void SetTPCClusterMin(int val) { fTCMinClusTPC = val; }
  void SetITSHitsMin(int val) { fTCMinHitsITS = val; }
  void SetMaxChi2perNDF(double val) { fTCMaxChi2pNDF = val; }
  void SetMaxIPxy(double val) { fTCMaxIPxy = val; }
  void SetMaxIPz(double val) { fTCMaxIPz = val; }
  void SetMaxbDecayLength(double val) { fTCMaxDecayLength = val; }
  void SetMaxDCATrackJet(double val) { fTCMaxDCATrackJet = val; }

  void SetFillControlHistos(bool fillHist) { fFillControlHists = fillHist; }

private:
  bool CalculateJetSignedTrackImpactParameter(AliAODTrack *track, AliEmcalJet *jet, double *impar, double *cov, double &sign, double &dcajetrack, double &lineardecaylength);
  double GetValImpactParameter(TTypeImpPar type, double *impar, double *cov);

  bool IsTrackAcceptedQuality(AliAODTrack *track, AliEmcalJet *Jet, int &QualityClass, double *imp, double *cov, double &sign);
  bool IsTrackAcceptedBJetCuts(AliAODTrack *track, int jetFlavour);

  bool MatchJetsGeometricDefault();                                                        // jet matching function 1/4
  void DoJetLoop();                                                                        // jet matching function 2/4
  void SetMatchingLevel(AliEmcalJet *jet1, AliEmcalJet *jet2, int matching = 0);           // jet matching function 3/4
  void GetGeometricalMatchingLevel(AliEmcalJet *jet1, AliEmcalJet *jet2, double &d) const; // jet matching function 4/4

  double CalculateTrackProb(double significance, int trclass, int jetFlavor);
  double CalculateJetProb(AliEmcalJet *jet, int jetFlavor); //!
  void FillResolutionFunctionHists(AliAODTrack *track, AliEmcalJet *jet, int jetFlavor);

  void MakeControlHistograms();

  TClonesArray *fInputList;   //! HF Jet List
  TClonesArray *fInputListb;  //! b-jet list
  TClonesArray *fInputListc;  //! c-jet list
  TClonesArray *fInputListlf; //! lf-jet list

  std::unique_ptr<AliHFJetsTagging> fHFJetUtils; //!

  bool fFillControlHists;    //
  double fThresholdIP;       //
  double fMaxDespersion;     //
  double fThresholdLxy;      //
  bool fEmbeddPerpendicular; // EMB_clus use perpendicular track embedding
  unsigned int fEvtNum;      //

  AliESDVertex *fDiamond;       //!
  AliVertexerTracks *fVertexer; //!

  TRandom3 *fRandom;             //! Random cone input
  AliFJWrapper *fFastJetWrapper; ///< EMB_clus wrapper for fast jet finding
  TRandom *fTrackGenerator;      ///< EMB_clus generator for track perpendicular to signal jet
  TClonesArray *fMCArray;        //!

  // Bjet cuts
  double fTCMinTrackPt;     //
  int fTCMinClusTPC;        //
  int fTCMinHitsITS;        //
  double fTCMaxChi2pNDF;    //
  double fTCMaxIPxy;        //
  double fTCMaxIPz;         //
  double fTCMaxDecayLength; //
  double fTCMaxDCATrackJet; //

  TH1D *fhistInclusiveJetCuts; //!
  TH1D *fhistbJetCuts;         //!
  TH1D *fhistcJetCuts;         //!
  TH1D *fhistlfJetCuts;        //!

  TH1D *fh1dJetRecPtAcceptedunCorr; //!

  TH2D *f2histRhoVsDeltaPt;       //!
  TH2D *f2histRhoVsDeltaPtFirst;  //!
  TH2D *f2histRhoVsDeltaPtSecond; //!
  TH2D *f2histRhoVsDeltaPtThird;  //!

  TH2D *fh2DeltaPtEmbeddCorrelationPerpendicular; //!

  TH1D *fh1dJetGenPt;             //! Generator level jet momentum for unfolding
  TH1D *fh1dJetGenPtUnidentified; //!
  TH1D *fh1dJetGenPtudsg;         //!
  TH1D *fh1dJetGenPtc;            //!
  TH1D *fh1dJetGenPtb;            //!

  TH1I *fh1dJetNCanditate;             //! Number of selected candidate jets
  TH1I *fh1dJetNCanditateUnidentified; //!
  TH1I *fh1dJetNCanditateudsg;         //!
  TH1I *fh1dJetNCanditatec;            //!
  TH1I *fh1dJetNCanditateb;            //!

  TH1D *fh1dJetRecPt;             //! Detector level jets
  TH1D *fh1dJetRecPtAccepted;     //! Detector level jets accepted
  TH2D *fh1dJetRecEtaPhiAccepted; //! Detector level jets accepted

  TH1D *fh1dJetRecPtUnidentified;         //!
  TH1D *fh1dJetRecPtudsg;                 //!
  TH1D *fh1dJetRecPtc;                    //!
  TH1D *fh1dJetRecPtb;                    //!
  TH1D *fh1dJetRecPtUnidentifiedAccepted; //!
  TH1D *fh1dJetRecPtudsgAccepted;         //!
  TH1D *fh1dJetRecPtcAccepted;            //!
  TH1D *fh1dJetRecPtbAccepted;            //!

  TH2D *fh2dJetGenPtVsJetRecPt;     //! raw momentum response matrix
  TH2D *fh2dJetGenPtVsJetRecPtb;    //! b momentum response matrix
  TH2D *fh2dJetGenPtVsJetRecPtc;    //! c momentum response matrix
  TH2D *fh2dJetGenPtVsJetRecPtudsg; //! udsg momentum response matrix

  TH2D *fh2dJetSignedImpParXY;             //!
  TH2D *fh2dJetSignedImpParXYUnidentified; //!
  TH2D *fh2dJetSignedImpParXYudsg;         //!
  TH2D *fh2dJetSignedImpParXYb;            //!
  TH2D *fh2dJetSignedImpParXYc;            //!

  TH2D *fh2dJetSignedImpParXYSignificance;             //!
  TH2D *fh2dJetSignedImpParXYSignificanceUnidentified; //!
  TH2D *fh2dJetSignedImpParXYSignificanceudsg;         //!
  TH2D *fh2dJetSignedImpParXYSignificanceb;            //!
  TH2D *fh2dJetSignedImpParXYSignificancec;            //!

  TH2D *fh2dJetSignedImpParXYZ;             //!
  TH2D *fh2dJetSignedImpParXYZUnidentified; //!
  TH2D *fh2dJetSignedImpParXYZudsg;         //!
  TH2D *fh2dJetSignedImpParXYZb;            //!
  TH2D *fh2dJetSignedImpParXYZc;            //!

  TH2D *fh2dJetSignedImpParXYZSignificance;             //!
  TH2D *fh2dJetSignedImpParXYZSignificanceUnidentified; //!
  TH2D *fh2dJetSignedImpParXYZSignificanceudsg;         //!
  TH2D *fh2dJetSignedImpParXYZSignificanceb;            //!
  TH2D *fh2dJetSignedImpParXYZSignificancec;            //!

  TH2D *fh2dJetSignedImpParXYSignificanceFirst;             //!
  TH2D *fh2dJetSignedImpParXYSignificanceUnidentifiedFirst; //!
  TH2D *fh2dJetSignedImpParXYSignificanceudsgFirst;         //!
  TH2D *fh2dJetSignedImpParXYSignificancebFirst;            //!
  TH2D *fh2dJetSignedImpParXYSignificancecFirst;            //!

  // Second
  TH2D *fh2dJetSignedImpParXYSignificanceSecond;             //!
  TH2D *fh2dJetSignedImpParXYSignificanceUnidentifiedSecond; //!
  TH2D *fh2dJetSignedImpParXYSignificanceudsgSecond;         //!
  TH2D *fh2dJetSignedImpParXYSignificancebSecond;            //!
  TH2D *fh2dJetSignedImpParXYSignificancecSecond;            //!

  // Third
  TH2D *fh2dJetSignedImpParXYSignificanceThird;             //!
  TH2D *fh2dJetSignedImpParXYSignificanceUnidentifiedThird; //!
  TH2D *fh2dJetSignedImpParXYSignificanceudsgThird;         //!
  TH2D *fh2dJetSignedImpParXYSignificancebThird;            //!
  TH2D *fh2dJetSignedImpParXYSignificancecThird;            //!

  // ################################ Histograms for resolution functions
  TH2D *fh2dJetSignedImpParXYSignificance_Class1;   //!
  TH2D *fh2dJetSignedImpParXYSignificanceb_Class1;  //!
  TH2D *fh2dJetSignedImpParXYSignificancec_Class1;  //!
  TH2D *fh2dJetSignedImpParXYSignificancelf_Class1; //!

  TH2D *fh2dJetSignedImpParXYSignificance_Class2;   //!
  TH2D *fh2dJetSignedImpParXYSignificanceb_Class2;  //!
  TH2D *fh2dJetSignedImpParXYSignificancec_Class2;  //!
  TH2D *fh2dJetSignedImpParXYSignificancelf_Class2; //!

  TH2D *fh2dJetSignedImpParXYSignificance_Class3;   //!
  TH2D *fh2dJetSignedImpParXYSignificanceb_Class3;  //!
  TH2D *fh2dJetSignedImpParXYSignificancec_Class3;  //!
  TH2D *fh2dJetSignedImpParXYSignificancelf_Class3; //!

  TH2D *fh2dJetSignedImpParXYSignificance_Class4;   //!
  TH2D *fh2dJetSignedImpParXYSignificanceb_Class4;  //!
  TH2D *fh2dJetSignedImpParXYSignificancec_Class4;  //!
  TH2D *fh2dJetSignedImpParXYSignificancelf_Class4; //!

  // Jet Probability Histograms
  TH2D *fhistJetProbability;              //!
  TH2D *fhistJetProbability_Unidentified; //!
  TH2D *fhistJetProbability_udsg;         //!
  TH2D *fhistJetProbability_c;            //!
  TH2D *fhistJetProbability_b;            //!

  TH2D *fhistJetProbabilityLog;              //!
  TH2D *fhistJetProbability_UnidentifiedLog; //!
  TH2D *fhistJetProbability_udsgLog;         //!
  TH2D *fhistJetProbability_cLog;            //!
  TH2D *fhistJetProbability_bLog;            //!

  TH2D *fhistJetProbabilityLogFirst;              //!
  TH2D *fhistJetProbability_UnidentifiedLogFirst; //!
  TH2D *fhistJetProbability_udsgLogFirst;         //!
  TH2D *fhistJetProbability_cLogFirst;            //!
  TH2D *fhistJetProbability_bLogFirst;            //!

  TH2D *fhistJetProbabilityLogSecond;              //!
  TH2D *fhistJetProbability_UnidentifiedLogSecond; //!
  TH2D *fhistJetProbability_udsgLogSecond;         //!
  TH2D *fhistJetProbability_cLogSecond;            //!
  TH2D *fhistJetProbability_bLogSecond;            //!

  TH2D *fhistJetProbabilityLogThird;              //!
  TH2D *fhistJetProbability_UnidentifiedLogThird; //!
  TH2D *fhistJetProbability_udsgLogThird;         //!
  TH2D *fhistJetProbability_cLogThird;            //!
  TH2D *fhistJetProbability_bLogThird;            //!

  TF1 *fResolutionFunction[7];   //
  TF1 *fResolutionFunctionb[7];  //
  TF1 *fResolutionFunctionc[7];  //
  TF1 *fResolutionFunctionlf[7]; //

  AliJetContainer *fJetContainerMC;   //! Container with reconstructed jets
  AliJetContainer *fJetContainerData; //! Container with reconstructed jets
  AliAODEvent *fAODIn;                //! AOD Input Event
  AliAODVertex *fPrimaryVertex;       //! Event Primary Vertex

  // Secondary Vertex
  bool fDoSVAnalysis;             //
  bool fDo3DSVHists;              // Fill the 3d SV histograms instead of the 2D
  bool fDoTrackCountingAnalysis;  //
  bool fUsePartonDef;             //
  bool fDoJetProbabilityAnalysis; //

  double fMinTrackProb;

  AliHFJetsTaggingVertex *fVtxTagger3Prong; //!
  AliHFJetsTaggingVertex *fVtxTagger2Prong; //!

  std::unique_ptr<AliRDHFJetsCutsVertex> fjetCuts3Prong; //! SV cuts
  std::unique_ptr<AliRDHFJetsCutsVertex> fjetCuts2Prong; //! SV cuts

  AliESDtrackCuts *fEsdTrackCuts; //! cuts used on the track selected for the SV reconstruction

  TH3D *fHistSV2Prong;             //!
  TH3D *fHistSV2ProngUnidentified; //!
  TH3D *fHistSV2Prongb;            //!
  TH3D *fHistSV2Prongc;            //!
  TH3D *fHistSV2Pronglf;           //!

  TH2D *fHistDecayLengthSV2Prong;             //!
  TH2D *fHistDecayLengthSV2ProngUnidentified; //!
  TH2D *fHistDecayLengthSV2Prongb;            //!
  TH2D *fHistDecayLengthSV2Prongc;            //!
  TH2D *fHistDecayLengthSV2Pronglf;           //!

  TH2D *fHistMassSV2Prong;             //!
  TH2D *fHistMassSV2ProngUnidentified; //!
  TH2D *fHistMassSV2Prongb;            //!
  TH2D *fHistMassSV2Prongc;            //!
  TH2D *fHistMassSV2Pronglf;           //!

  TH2D *fHistDispersion2Prong;             //!
  TH2D *fHistDispersion2ProngUnidentified; //!
  TH2D *fHistDispersion2Prongb;            //!
  TH2D *fHistDispersion2Prongc;            //!
  TH2D *fHistDispersion2Pronglf;           //!

  TH3D *fHistSV3Prong;             //!
  TH3D *fHistSV3ProngUnidentified; //!
  TH3D *fHistSV3Prongb;            //!
  TH3D *fHistSV3Prongc;            //!
  TH3D *fHistSV3Pronglf;           //!

  TH2D *fHistDecayLengthSV3Prong;             //!
  TH2D *fHistDecayLengthSV3ProngUnidentified; //!
  TH2D *fHistDecayLengthSV3Prongb;            //!
  TH2D *fHistDecayLengthSV3Prongc;            //!
  TH2D *fHistDecayLengthSV3Pronglf;           //!

  TH2D *fHistMassSV3Prong;             //!
  TH2D *fHistMassSV3ProngUnidentified; //!
  TH2D *fHistMassSV3Prongb;            //!
  TH2D *fHistMassSV3Prongc;            //!
  TH2D *fHistMassSV3Pronglf;           //!

  TH2D *fHistDispersion3Prong;             //!
  TH2D *fHistDispersion3ProngUnidentified; //!
  TH2D *fHistDispersion3Prongb;            //!
  TH2D *fHistDispersion3Prongc;            //!
  TH2D *fHistDispersion3Pronglf;           //!

  ClassDef(AliJHFTagging, 1);
};
#endif // AliJHFTagging_H
