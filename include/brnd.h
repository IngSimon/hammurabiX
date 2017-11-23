///
/// iso/aniso-tropic turbulent Galactic magnetic field generators
///
#ifndef HAMMURABI_BRND_H
#define HAMMURABI_BRND_H

#include <memory>
#include <fftw3.h>
#include <vec3.h>
#include <vector>
#include <gsl/gsl_integration.h>
#include "pond.h"
#include "grid.h"
#include "cgs_units_file.h"
#include "breg.h"

///
/// base class with \p read_grid implemented,
/// \p get_brnd is invoked when no specific derived class object is instantiated
///
class Brnd{
public:
    Brnd(void) = default;
    virtual ~Brnd(void) = default;
    ///
    /// return empty field without invoking \p read_grid
    ///
    virtual vec3_t<double> get_brnd(const vec3_t<double> &,Grid_brnd *);
    ///
    /// read field from grid with trilinear interpolation,
    /// user has to call \p write_grid ahead
    ///
    virtual vec3_t<double> read_grid(const vec3_t<double> &,Grid_brnd *);
    ///
    /// write field to grid (model dependent),
    /// user can \p export_grid to binary file with \p Grid_xxx::export_grid
    ///
    virtual void write_grid_iso(Pond *,Grid_brnd *);
    ///
    /// for dynamic binding only, implemented in derived class
    ///
    virtual void write_grid_ani(Pond *,Breg *,Grid_breg *,Grid_brnd *);
};

///
/// global isotropic turbulent GMF with strength rescaling
///
class Brnd_iso : public Brnd{
public:
    Brnd_iso(void) = default;
    virtual ~Brnd_iso(void) = default;
    vec3_t<double> get_brnd(const vec3_t<double> &,Grid_brnd *) override;
    ///
    /// global isotropic generator, using triple Fourier transform scheme
    ///
    void write_grid_iso(Pond *,Grid_brnd *) override;
    ///
    /// isotropic power-spectrum
    ///
    virtual double b_spec(const double &,Pond *);
    ///
    /// field energy density rescaling factor
    ///
    virtual double rescal_fact(const vec3_t<double> &,Pond *);
    
protected:
    ///
    /// get real part of each element from a complex array
    ///
    void complex2real(const fftw_complex *,double *,const std::size_t &);
    ///
    /// Gram-Schmidt orthogonalization process
    ///
    vec3_t<double> gramschmidt(const vec3_t<double> &,const vec3_t<double> &);
};

///
/// global anisotropic turbulent GMF,
/// inheret isotropic spectrum and density rescaling from \p Brnd_iso class
///
class Brnd_anig final : public Brnd_iso{
public:
    Brnd_anig(void) = default;
    virtual ~Brnd_anig(void) = default;
    ///
    /// triple Fourier transform scheme
    ///
    void write_grid_ani(Pond *,Breg *,Grid_breg *,Grid_brnd *) override;
    
private:
    ///
    /// anisotropy factor at given point
    ///
    double anisotropy(const vec3_t<double> &,vec3_t<double> &,Pond *,Breg *,Grid_breg *);
};

///
/// local anisotropic turbulent GMF, in compressive MHD plasma
///
class Brnd_anil final : public Brnd{
public:
    Brnd_anil(void) = default;
    virtual ~Brnd_anil(void) = default;
    vec3_t<double> get_brnd(const vec3_t<double> &,Grid_brnd *) override;
    ///
    /// vector field decomposition scheme,
    /// using regular GMF at Sun position
    ///
    void write_grid_ani(Pond *,Breg *,Grid_breg *,Grid_brnd *) override;
    
private:
    ///
    /// dynamo number
    ///
    double dynamo(const double &,const double &);
    ///
    /// fast mode anisotropic tensor structure
    ///
    double hf(const double &,const double &);
    ///
    /// slow mode anisotropic tensor structure
    ///
    double hs(const double &,const double &);
    ///
    /// fast mode anisotropy power factor
    ///
    double fa(const double &,const double &);
    ///
    /// slow mode anisotropy power factor
    ///
    double fs(const double &,const double &);
    
    ///
    /// cosine alpha, where alpha is pitch angle between wavevector and regular GMF
    ///
    double cosa(const vec3_t<double> &,const vec3_t<double> &);
    ///
    /// direction of Alfven mode
    ///
    vec3_t<double> eplus(const vec3_t<double> &,const vec3_t<double> &);
    ///
    /// direction of slow and fast modes
    ///
    vec3_t<double> eminus(const vec3_t<double> &,const vec3_t<double> &);
    
    ///
    /// isotropic part of power spectrum of Alfvenic mode
    ///
    double speca(const double &,Pond *);
    ///
    /// isotropic part of power spectrum of fast mode
    ///
    double specf(const double &,Pond *);
    ///
    /// isotropic part of power spectrum of slow mode
    ///
    double specs(const double &,Pond *);
    
    void complex2real(const fftw_complex *,double *,const std::size_t &);
};

#endif

// END
