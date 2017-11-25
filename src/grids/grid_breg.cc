#include <sstream>
#include <iostream>
#include <memory>
#include <fftw3.h>
#include <array>
#include <string>
#include <vector>
#include <tinyxml2.h>
#include <fitshandle.h>
#include <fstream>
#include <healpix_map_fitsio.h>
#include <fitsio.h>
#include "grid.h"
#include "cgs_units_file.h"
#include "namespace_toolkit.h"

using namespace tinyxml2;
using namespace std;

Grid_breg::Grid_breg(string file_name){
    unique_ptr<XMLDocument> doc = unique_ptr<XMLDocument> (new XMLDocument());
    doc->LoadFile(file_name.c_str());
    XMLElement *ptr {doc->FirstChildElement("root")->FirstChildElement("Fieldout")->FirstChildElement("breg_grid")};
    read_permission = ptr->BoolAttribute("read");
    write_permission = ptr->BoolAttribute("write");
    // build up grid when have read or write permission
    if(read_permission or write_permission){
#ifndef NDEBUG
        cout<<"IFNO: GRID_BREG I/O ACTIVE"<<endl;
#endif
        filename = ptr->Attribute("filename");
        build_grid(doc.get());
    }
}

void Grid_breg::build_grid(XMLDocument *doc){
    XMLElement *ptr {doc->FirstChildElement("root")->FirstChildElement("Grid")->FirstChildElement("Box")};
    // Cartesian grid
    nx = FetchUnsigned(ptr,"nx");
    ny = FetchUnsigned(ptr,"ny");
    nz = FetchUnsigned(ptr,"nz");
    full_size = nx*ny*nz;
    // box limit for filling field
    x_max = CGS_U_kpc*FetchDouble(ptr,"x_max");
    x_min = CGS_U_kpc*FetchDouble(ptr,"x_min");
    y_max = CGS_U_kpc*FetchDouble(ptr,"y_max");
    y_min = CGS_U_kpc*FetchDouble(ptr,"y_min");
    z_max = CGS_U_kpc*FetchDouble(ptr,"z_max");
    z_min = CGS_U_kpc*FetchDouble(ptr,"z_min");
#ifndef NDEBUG
    // memory check (double complex + double + double)
    const double bytes {full_size*(3.*8.)};
    cout<<"INFO: BREG REQUIRING "<<bytes/1.e9<<" GB MEMORY"<<endl;
#endif
    // real 3D regular b field
    reg_b_x = unique_ptr<double[]> (new double[full_size]);
    reg_b_y = unique_ptr<double[]> (new double[full_size]);
    reg_b_z = unique_ptr<double[]> (new double[full_size]);
}

void Grid_breg::export_grid(void){
    if(filename.empty()){
        cerr<<"ERR:"<<__FILE__
        <<" : in function "<<__func__<<endl
        <<" at line "<<__LINE__<<endl
        <<"NONEXIST FILE"<<endl;
        exit(1);
    }
    ofstream output(filename.c_str(), std::ios::out|std::ios::binary);
    if (!output.is_open()){
        cerr<<"ERR:"<<__FILE__
        <<" : in function "<<__func__<<endl
        <<" at line "<<__LINE__<<endl
        <<"COULD NOT OPEN: "<<filename<<endl;
        exit(1);
    }
    double tmp;
    for(decltype(full_size) i=0;i!=full_size;++i){
        if (output.eof()) {
            cerr<<"ERR:"<<__FILE__
            <<" : in function "<<__func__<<endl
            <<" at line "<<__LINE__<<endl
            <<"UNEXPECTED END AT: "<<i<<endl;
            exit(1);
        }
        tmp = reg_b_x[i];
        output.write(reinterpret_cast<char*>(&tmp),sizeof(double));
        tmp = reg_b_y[i];
        output.write(reinterpret_cast<char*>(&tmp),sizeof(double));
        tmp = reg_b_z[i];
        output.write(reinterpret_cast<char*>(&tmp),sizeof(double));
    }
    output.close();
    // exit program
#ifndef NDEBUG
    cout<<"...REGULAR MAGNETIC FIELD EXPORTED AND CLEANED..."<<endl;
#endif
    exit(0);
}

void Grid_breg::import_grid(void){
    if(filename.empty()){
        cerr<<"ERR:"<<__FILE__
        <<" : in function "<<__func__<<endl
        <<" at line "<<__LINE__<<endl
        <<"NONEXIST FILE"<<endl;
        exit(1);
    }
    ifstream input(filename.c_str(), std::ios::in|std::ios::binary);
    if (!input.is_open()){
        cerr<<"ERR:"<<__FILE__
        <<" : in function "<<__func__<<endl
        <<" at line "<<__LINE__<<endl
        <<"COULD NOT OPEN: "<<filename<<endl;
        exit(1);
    }
    double tmp;
    for(decltype(full_size) i=0;i!=full_size;++i){
        if (input.eof()) {
            cerr<<"ERR:"<<__FILE__
            <<" : in function "<<__func__<<endl
            <<" at line "<<__LINE__<<endl
            <<"UNEXPECTED END AT: "<<i<<endl;
            exit(1);
        }
        input.read(reinterpret_cast<char *>(&tmp),sizeof(double));
        reg_b_x[i] = tmp;
        input.read(reinterpret_cast<char *>(&tmp),sizeof(double));
        reg_b_y[i] = tmp;
        input.read(reinterpret_cast<char *>(&tmp),sizeof(double));
        reg_b_z[i] = tmp;
    }
    auto eof = input.tellg();
    input.seekg (0, input.end);
    if (eof != input.tellg()){
        cerr<<"ERR:"<<__FILE__
        <<" : in function "<<__func__<<endl
        <<" at line "<<__LINE__<<endl
        <<"INCORRECT LENGTH"<<endl;
        exit(1);
    }
    input.close();
}
