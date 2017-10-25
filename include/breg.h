/*
 *@file: class_breg.h
 *@author: Jiaxin Wang
 *@email: jiwang@sissa.it
 *@brief: regular magnetic field generator
 */
#ifndef HAMMURABI_BREG_H
#define HAMMURABI_BREG_H

#include <fftw3.h>
#include <vec3.h>
#include <vector>
#include "pond.h"
#include "grid.h"

class Breg{
public:
    Breg(void) = default;
    virtual ~Breg(void) = default;
    /*
     *@get_breg
     * get regular magnetic field at gc position
     * read_grid regardless of field type if permitted
     * or calculate directly
     */
    virtual vec3 get_breg(const vec3 &,Pond *,Grid_breg *);
    /*@breg
     * regular field builder specified in derived class
     */
    virtual vec3 breg(const vec3 &,Pond *);
    /*@read_grid
     * read from grid with trilinear interpolation
     */
    virtual vec3 read_grid(const vec3 &,Grid_breg *);
    /*@write_grid
     * write to grid
     */
    virtual void write_grid(Pond *,Grid_breg *);
};

// verify
class Breg_verify final : public Breg{
public:
    Breg_verify(void) = default;
    virtual ~Breg_verify(void) = default;
    vec3 breg(const vec3 &,Pond *) override;
};

// WMAP-3yr
class Breg_wmap final : public Breg {
public:
    Breg_wmap(void) = default;
    virtual ~Breg_wmap(void) = default;
    vec3 breg(const vec3 &,Pond *) override;
};

#endif

// END
