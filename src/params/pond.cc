#include <string>
#include <vec3.h>
#include <tinyxml2.h>
#include "pond.h"
#include "cgs_units_file.h"

using namespace tinyxml2;
using namespace std;

Pond::Pond(std::string file_name){
    XMLDocument *doc = new XMLDocument();
    doc->LoadFile(file_name.c_str());
    // gc sun position
    XMLElement *ptr {doc->FirstChildElement("root")->FirstChildElement("Grid")->FirstChildElement("SunPosition")};
    SunPosition = vec3_t<double> {CGS_U_kpc*FetchDouble(ptr,"x"),
        CGS_U_kpc*FetchDouble(ptr,"y"),
        CGS_U_pc*FetchDouble(ptr,"z")};
    
    b_pond(doc);
    fe_pond(doc);
    cre_pond(doc);
    
    delete doc;
}

// auxiliary functions
std::string Pond::FetchString(XMLElement* el, string obj){
    return el->FirstChildElement(obj.c_str())->Attribute("value");
}

int Pond::FetchInt(XMLElement* el, string obj){
    return el->FirstChildElement(obj.c_str())->IntAttribute("value");
}

unsigned int Pond::FetchUnsigned(XMLElement* el, string obj){
    return el->FirstChildElement(obj.c_str())->UnsignedAttribute("value");
}

bool Pond::FetchBool(XMLElement* el, string obj){
    return el->FirstChildElement(obj.c_str())->BoolAttribute("cue");
}

double Pond::FetchDouble(XMLElement* el, string obj){
    return el->FirstChildElement(obj.c_str())->DoubleAttribute("value");
}

// magnetic field
void Pond::b_pond(XMLDocument *doc){
    XMLElement *ptr {doc->FirstChildElement("root")->FirstChildElement("Galaxy")->FirstChildElement("MagneticField")};
    // bwmap
    XMLElement *subptr {ptr->FirstChildElement("Regular")->FirstChildElement("WMAP")};
    breg_wmap.b0 = FetchDouble(subptr,"b0")*CGS_U_muGauss; //microGauss
    breg_wmap.psi0 = FetchDouble(subptr,"psi0")*CGS_U_rad; //rad
    breg_wmap.psi1 = FetchDouble(subptr,"psi1")*CGS_U_rad; //rad
    breg_wmap.chi0 = FetchDouble(subptr,"chi0")*CGS_U_rad; //rad
    // bverify
    subptr = ptr->FirstChildElement("Regular")->FirstChildElement("Verify");
    breg_verify.b0 = FetchDouble(subptr,"b0")*CGS_U_muGauss; //microGauss
    breg_verify.l0 = FetchDouble(subptr,"l0")*CGS_U_rad; //rad
    breg_verify.r = FetchDouble(subptr,"r");
    // bjaffe
    subptr = ptr->FirstChildElement("Regular")->FirstChildElement("Jaffe");
    breg_jaffe.quadruple = FetchBool(subptr,"quadruple");
    breg_jaffe.bss = FetchBool(subptr,"bss");
    breg_jaffe.disk_amp = FetchDouble(subptr,"disk_amp")*CGS_U_muGauss; //microG
    breg_jaffe.disk_z0 = FetchDouble(subptr,"disk_z0")*CGS_U_kpc; //kpc
    breg_jaffe.halo_amp = FetchDouble(subptr,"halo_amp")*CGS_U_muGauss; //microG
    breg_jaffe.halo_z0 = FetchDouble(subptr,"halo_z0")*CGS_U_kpc; //kpc
    breg_jaffe.r_inner = FetchDouble(subptr,"r_inner")*CGS_U_kpc; //kpc
    breg_jaffe.r_scale = FetchDouble(subptr,"r_scale")*CGS_U_kpc; //kpc
    breg_jaffe.r_peak = FetchDouble(subptr,"r_peak")*CGS_U_kpc; //kpc
    breg_jaffe.ring = FetchBool(subptr,"ring");
    breg_jaffe.ring_amp = FetchDouble(subptr,"ring_amp")*CGS_U_muGauss; //microG
    breg_jaffe.ring_r = FetchDouble(subptr,"ring_r")*CGS_U_kpc; //kpc
    breg_jaffe.bar = FetchBool(subptr,"bar");
    breg_jaffe.bar_amp = FetchDouble(subptr,"bar_amp")*CGS_U_muGauss; //microG
    breg_jaffe.bar_a = FetchDouble(subptr,"bar_a")*CGS_U_kpc; //kpc
    breg_jaffe.bar_b = FetchDouble(subptr,"bar_b")*CGS_U_kpc; //kpc
    breg_jaffe.bar_phi0 = FetchDouble(subptr,"bar_phi0")*CGS_U_rad; //rad
    breg_jaffe.arm_num = FetchUnsigned(subptr,"arm_num");
    breg_jaffe.arm_r0 = FetchDouble(subptr,"arm_r0")*CGS_U_kpc; //kpc
    breg_jaffe.arm_z0 = FetchDouble(subptr,"arm_z0")*CGS_U_kpc; //kpc
    breg_jaffe.arm_phi0.push_back(FetchDouble(subptr,"arm_phi1")*CGS_U_rad); //rad
    breg_jaffe.arm_phi0.push_back(FetchDouble(subptr,"arm_phi2")*CGS_U_rad); //rad
    breg_jaffe.arm_phi0.push_back(FetchDouble(subptr,"arm_phi3")*CGS_U_rad); //rad
    breg_jaffe.arm_phi0.push_back(FetchDouble(subptr,"arm_phi4")*CGS_U_rad); //rad
    breg_jaffe.arm_amp.push_back(FetchDouble(subptr,"arm_amp1")*CGS_U_muGauss); //microG
    breg_jaffe.arm_amp.push_back(FetchDouble(subptr,"arm_amp2")*CGS_U_muGauss); //microG
    breg_jaffe.arm_amp.push_back(FetchDouble(subptr,"arm_amp3")*CGS_U_muGauss); //microG
    breg_jaffe.arm_amp.push_back(FetchDouble(subptr,"arm_amp4")*CGS_U_muGauss); //microG
    breg_jaffe.arm_pitch = FetchDouble(subptr,"arm_pitch")*CGS_U_rad; //rad
    breg_jaffe.comp_c = FetchDouble(subptr,"comp_c");
    breg_jaffe.comp_d = FetchDouble(subptr,"comp_d")*CGS_U_kpc; //kpc
    breg_jaffe.comp_r = FetchDouble(subptr,"comp_r")*CGS_U_kpc; //kpc
    breg_jaffe.comp_p = FetchDouble(subptr,"comp_p");
    // random seed
    brnd_seed = ptr->FirstChildElement("Random")->UnsignedAttribute("seed");
    // brnd_iso
    subptr = ptr->FirstChildElement("Random")->FirstChildElement("Iso");
    brnd_iso.rms = FetchDouble(subptr,"rms")*CGS_U_muGauss; //microG^2;
    // turning point
    brnd_iso.k0 = FetchDouble(subptr,"k0");
    // index before turning (large scales)
    brnd_iso.a0 = FetchDouble(subptr,"a0");
    // brnd_aniso_glob
    subptr = ptr->FirstChildElement("Random")->FirstChildElement("Anisoglob");
    brnd_anig.rho = FetchDouble(subptr,"rho");
    // brnd_aniso_local
    subptr = ptr->FirstChildElement("Random")->FirstChildElement("Anisolocal");
    brnd_anil.p0 = FetchDouble(subptr,"p0")*CGS_U_muGauss*CGS_U_muGauss;
    brnd_anil.rf = FetchDouble(subptr,"rf");
    brnd_anil.rs = FetchDouble(subptr,"rs");
    
    brnd_anil.aa0 = FetchDouble(subptr,"aa0");
    brnd_anil.af0 = FetchDouble(subptr,"af0");
    brnd_anil.as0 = FetchDouble(subptr,"as0");
    brnd_anil.k0 = FetchDouble(subptr,"k0");
    
    brnd_anil.beta = FetchDouble(subptr,"beta");
    brnd_anil.ma = FetchDouble(subptr,"ma");
    // rescaling parameters
    subptr = ptr->FirstChildElement("Random")->FirstChildElement("Rescal");
    brnd_scal.r0 = FetchDouble(subptr,"r0")*CGS_U_kpc;
    brnd_scal.z0 = FetchDouble(subptr,"z0")*CGS_U_kpc;
}

void Pond::fe_pond(XMLDocument *doc){
    XMLElement *ptr {doc->FirstChildElement("root")->FirstChildElement("Galaxy")->FirstChildElement("FreeElectron")};
    // YMW16
    // Warp_Sun
    XMLElement *subptr {ptr->FirstChildElement("Regular")->FirstChildElement("YMW16")->FirstChildElement("Warp")};
    fereg_ymw16.R_warp = FetchDouble(subptr,"R_warp")*CGS_U_kpc; //kpc
    fereg_ymw16.R0 = FetchDouble(subptr,"R0")*CGS_U_kpc; //kpc
    fereg_ymw16.t0_Gamma_w = FetchDouble(subptr,"Gamma_w");
    // thick disk
    subptr = ptr->FirstChildElement("Regular")->FirstChildElement("YMW16")->FirstChildElement("ThickDisk");
    fereg_ymw16.t1_Ad = FetchDouble(subptr,"Ad")*CGS_U_pc;//pc
    fereg_ymw16.t1_Bd = FetchDouble(subptr,"Bd")*CGS_U_pc;//pc
    fereg_ymw16.t1_n1 = FetchDouble(subptr,"n1");//pccm
    fereg_ymw16.t1_H1 = FetchDouble(subptr,"H1")*CGS_U_pc;//pc
    // thin disk
    subptr = ptr->FirstChildElement("Regular")->FirstChildElement("YMW16")->FirstChildElement("ThinDisk");
    fereg_ymw16.t2_A2 = FetchDouble(subptr,"A2")*CGS_U_pc;//pc
    fereg_ymw16.t2_B2 = FetchDouble(subptr,"B2")*CGS_U_pc;//pc
    fereg_ymw16.t2_n2 = FetchDouble(subptr,"n2");//pccm
    fereg_ymw16.t2_K2 = FetchDouble(subptr,"K2");
    // spiral arm
    subptr = ptr->FirstChildElement("Regular")->FirstChildElement("YMW16")->FirstChildElement("SpiralArm");
    fereg_ymw16.t3_B2s = FetchDouble(subptr,"B2s")*CGS_U_pc; //pc
    fereg_ymw16.t3_narm[0] = FetchDouble(subptr,"Ele_arm_0");//pccm
    fereg_ymw16.t3_narm[1] = FetchDouble(subptr,"Ele_arm_1");
    fereg_ymw16.t3_narm[2] = FetchDouble(subptr,"Ele_arm_2");
    fereg_ymw16.t3_narm[3] = FetchDouble(subptr,"Ele_arm_3");
    fereg_ymw16.t3_narm[4] = FetchDouble(subptr,"Ele_arm_4");
    fereg_ymw16.t3_warm[0] = FetchDouble(subptr,"Wid_arm_0")*CGS_U_pc;//pc
    fereg_ymw16.t3_warm[1] = FetchDouble(subptr,"Wid_arm_1")*CGS_U_pc;
    fereg_ymw16.t3_warm[2] = FetchDouble(subptr,"Wid_arm_2")*CGS_U_pc;
    fereg_ymw16.t3_warm[3] = FetchDouble(subptr,"Wid_arm_3")*CGS_U_pc;
    fereg_ymw16.t3_warm[4] = FetchDouble(subptr,"Wid_arm_4")*CGS_U_pc;
    fereg_ymw16.t3_Aa = FetchDouble(subptr,"Aa")*CGS_U_pc;//pc
    fereg_ymw16.t3_Ka = FetchDouble(subptr,"Ka");
    fereg_ymw16.t3_ncn = FetchDouble(subptr,"ncn");
    fereg_ymw16.t3_thetacn = FetchDouble(subptr,"thetacn");//deg
    fereg_ymw16.t3_wcn = FetchDouble(subptr,"wcn");//deg
    fereg_ymw16.t3_nsg = FetchDouble(subptr,"nsg");
    fereg_ymw16.t3_thetasg = FetchDouble(subptr,"thetasg");//deg
    fereg_ymw16.t3_wsg = FetchDouble(subptr,"wsg");//deg
    // gc
    subptr = ptr->FirstChildElement("Regular")->FirstChildElement("YMW16")->FirstChildElement("GalCenter");
    fereg_ymw16.t4_ngc = FetchDouble(subptr,"ngc");//pccm
    fereg_ymw16.t4_Agc = FetchDouble(subptr,"Agc")*CGS_U_pc;//pc
    fereg_ymw16.t4_Hgc = FetchDouble(subptr,"Hgc")*CGS_U_pc;//pc
    // Gum
    subptr = ptr->FirstChildElement("Regular")->FirstChildElement("YMW16")->FirstChildElement("GumNebula");
    fereg_ymw16.t5_ngn = FetchDouble(subptr,"ngn");//pccm
    fereg_ymw16.t5_Wgn = FetchDouble(subptr,"Wgn")*CGS_U_pc;//pc
    fereg_ymw16.t5_Agn = FetchDouble(subptr,"Agn")*CGS_U_pc;//pc
    fereg_ymw16.t5_Kgn = FetchDouble(subptr,"Kgn");
    // Local Bubble
    subptr = ptr->FirstChildElement("Regular")->FirstChildElement("YMW16")->FirstChildElement("LocalBubble");
    fereg_ymw16.t6_J_LB = FetchDouble(subptr,"J_LB");
    fereg_ymw16.t6_nlb1 = FetchDouble(subptr,"nlb1");//pccm
    fereg_ymw16.t6_thetalb1 = FetchDouble(subptr,"thetalb1");//deg
    fereg_ymw16.t6_detlb1 = FetchDouble(subptr,"detlb1");//deg
    fereg_ymw16.t6_wlb1 = FetchDouble(subptr,"wlb1")*CGS_U_pc;//pc
    fereg_ymw16.t6_hlb1 = FetchDouble(subptr,"hlb1")*CGS_U_pc;//pc
    fereg_ymw16.t6_nlb2 = FetchDouble(subptr,"nlb2");//pccm
    fereg_ymw16.t6_thetalb2 = FetchDouble(subptr,"thetalb2");//deg
    fereg_ymw16.t6_detlb2 = FetchDouble(subptr,"detlb2");//deg
    fereg_ymw16.t6_wlb2 = FetchDouble(subptr,"wlb2")*CGS_U_pc;//pc
    fereg_ymw16.t6_hlb2 = FetchDouble(subptr,"hlb2")*CGS_U_pc;//pc
    // Loop I
    subptr = ptr->FirstChildElement("Regular")->FirstChildElement("YMW16")->FirstChildElement("LoopI");
    fereg_ymw16.t7_nLI = FetchDouble(subptr,"nLI");//pccm
    fereg_ymw16.t7_RLI = FetchDouble(subptr,"RLI")*CGS_U_pc;//pc
    fereg_ymw16.t7_WLI = FetchDouble(subptr,"WLI")*CGS_U_pc;//pc
    fereg_ymw16.t7_detthetaLI = FetchDouble(subptr,"detthetaLI");//deg
    fereg_ymw16.t7_thetaLI = FetchDouble(subptr,"thetaLI");//deg
    
    // verify
    subptr = ptr->FirstChildElement("Regular")->FirstChildElement("Verify");
    fereg_verify.n0 = FetchDouble(subptr,"n0");
    fereg_verify.r0 = FetchDouble(subptr,"r0")*CGS_U_kpc; //kpc
    
    // random seed
    fernd_seed = ptr->FirstChildElement("Random")->UnsignedAttribute("seed");
    // isotropic turbulent
    subptr = ptr->FirstChildElement("Random")->FirstChildElement("Iso");
    // nomalization
    fernd_iso.rms = FetchDouble(subptr,"rms");
    // turning point
    fernd_iso.k0 = FetchDouble(subptr,"k0");
    // index before turning (large scales)
    fernd_iso.a0 = FetchDouble(subptr,"a0");
    
    // rescaling parameters for random fe
    subptr = ptr->FirstChildElement("Random")->FirstChildElement("Rescal");
    fernd_scal.r0 = FetchDouble(subptr,"r0")*CGS_U_kpc; //kpc
    fernd_scal.z0 = FetchDouble(subptr,"z0")*CGS_U_kpc; //kpc
}

void Pond::cre_pond(XMLDocument *doc){
    XMLElement *ptr {doc->FirstChildElement("root")->FirstChildElement("CRE")};
    if (doc->FirstChildElement("root")->FirstChildElement("Output")->FirstChildElement("Sync")!=nullptr){
        sim_freq = doc->FirstChildElement("root")->FirstChildElement("Output")->FirstChildElement("Sync")->DoubleAttribute("freq")*CGS_U_GHz;
    }
    else{
        sim_freq = 0.;
    }
    // analytical
    XMLElement *subptr {ptr->FirstChildElement("Analytic")};
    // the default parameter setting follows wmap3yr model
    cre_ana.alpha = FetchDouble(subptr,"alpha");
    cre_ana.beta = FetchDouble(subptr,"beta");
    cre_ana.theta = FetchDouble(subptr,"theta");
    cre_ana.hr = FetchDouble(subptr,"hr")*CGS_U_kpc; //kpc
    cre_ana.hz = FetchDouble(subptr,"hz")*CGS_U_kpc; //kpc
    // differential flux normalization factor at gamma 10
    cre_ana.je = FetchDouble(subptr,"je");
    // verification
    subptr = ptr->FirstChildElement("Verify");
    cre_verify.alpha = FetchDouble(subptr,"alpha");
    cre_verify.r0 = FetchDouble(subptr,"r0")*CGS_U_kpc; //kpc
    cre_verify.je = FetchDouble(subptr,"je");
}

// END
