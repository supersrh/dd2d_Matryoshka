/**
 * @file polycrystal.h
 * @author Adhish Majumdar
 * @version 1.3
 * @date 03/12/2013
 * @brief Definition of the Polycrystal class which represents a polycrystal in the simulation.
 * @details This file declares member data and functions of the Polycrystal class representing an polycrystal in the simulation. A polycrystal is considered to be a collection of several grains.
 */

/*
    DD2D-Matryoshka approach.
    A set of classes defining the behaviour of crystalline defects,
    with the final goal of carrying out dislocation dynamics simulations
    in two dimensions.
    Copyright (C) 2013  Adhish Majumdar
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef POLYCRYSTAL_H
#define POLYCRYSTAL_H

#include "tess2d.h"
#include "grain.h"

/**
 * @brief The Polycrystal class.
 * @details This class represents a polycrystal in the simulation. A polycrystal is considered to be a collection of grains. Each grain has its grain boundary points and a crystallographic orientation.
 */
class Polycrystal
{
protected:
    /**
     * @brief STL vector container with pointers to the grains.
     * @details
     */
    std::vector<Grain*> grains;

    /**
     * @brief STL vector with the crystallographic orientations of the grains.
     */
    std::vector<Vector3d> orientations;

    /**
     * @brief The Voronoi tessellation that represents the microstructure in 2 dimensions.
     */
    Tess2d tessellation;

    /**
     * @brief The polycrystal's local co-ordinate system.
     * @details This variable holds the polycrystal's local co-ordinate system. This is the highest level in the simulation, so it will be a standard co-ordinate system with the standard axes, and the base pointer will be NULL.
     */
    CoordinateSystem coordinateSystem;

    /**
     * @brief The externally applied stress on the polycrystal, expressed in the base co-ordinate system.
     */
    Stress appliedStress_base;

    /**
     * @brief The externally applied stress on the polycrystal, expressed in the local co-ordinate system.
     */
    Stress appliedStress_local;

public:
    /**
     * @brief Default constructor for the Polycrystal class.
     */
    Polycrystal();

    /**
     * @brief Destructor for the Polycrystal class.
     */
    virtual ~Polycrystal();

    // Assignment functions
    /**
     * @brief Set the Voronoi tessellation for the polycrystal from the tessellation file provided.
     * @param tessellationFileName Name of the files containing the Voronoi tessellation.
     */
    void setTessellation (std::string tessellationFileName);

    /**
     * @brief Set the orientations vector by reading orientations from a file.
     * @param orientationsFileName Name of the file containing the orientations.
     */
    void setOrientations (std::string orientationsFileName);

    /**
     * @brief Initialize the Grain vector.
     */
    void initializeGrainVector ();

    /**
     * @brief Set the grain boundaries for each grain using data from the tessellation.
     */
    void setGrainBoundaries ();

    /**
     * @brief Set the grain orientations from the orientations list.
     */
    void setGrainOrientations ();

    /**
     * @brief Insert a new grain into the polycrystal.
     * @param g Pointer to the grain (instance of the class Grain).
     */
    void insertGrain (Grain* g);

    /**
     * @brief Set the applied stress on the polycrystal.
     * @param s The value of the applied stress.
     */
    void setAppliedStress (Stress s);

    // Access functions
    /**
     * @brief Get the pointer to a grain indicated by the index given as argument.
     * @details The grains are stored in memory and pointers to them are stored in the vector Polycrystal::grains. The index i provided as argument indicates which grain is required and the pointer to that grain is returned. If the index is invalid, NULL is returned.
     * @param i Index of the grain in the vector Polycrystal::grains.
     * @return Pointer to the grain at the position i.
     */
    Grain* getGrain (int i);

    /**
     * @brief Get a pointer to the Grain CoordinateSystem.
     * @return Pointer to the Grain co-ordinate system.
     */
    CoordinateSystem* getCoordinateSystem();

    /**
     * @brief Get the applied stress on the polycrystal, expressed in the base co-ordinate system.
     * @return The applied stress on the polycrystal, expressed in the base co-ordinate system.
     */
    Stress getAppliedStress_base () const;

    /**
     * @brief Get the applied stress on the polycrystal, expressed in the local co-ordinate system.
     * @return The applied stress on the polycrystal, expressed in the local co-ordinate system.
     */
    Stress getAppliedStress_local () const;

    // Stresses
    /**
     * @brief Calculate the applied stress on all the grains in the polycrystal.
     */
    void calculateGrainAppliedStress ();

    /**
     * @brief Calculate all the stresses in all the defects in the simulation.
     * @param mu Shear modulus (Pa).
     * @param nu Poisson's ratio.
     */
    void calculateAllStresses (double mu, double nu);

    /**
     * @brief Return the total stress at the point p.
     * @param p Position vector, in the polycrystal co-ordinate system where the stress is to be calculated.
     * @param mu Shear modulus (Pa).
     * @param nu Poisson's ratio.
     * @return
     */
    Stress totalStress (Vector3d p, double mu, double nu);

    // Iteration functions
    /**
     * @brief Calculate the Peach-Koehler force on all dislocations and their resulting velocities.
     * @param B The drag coefficient.
     */
    void calculateDislocationVelocities (double B);

    // Time increments
    /**
     * @brief Set the time increments on all the slip systems of all the grains.
     * @param dt The time increment.
     */
    void setGrainTimeIncrements (double dt);

    // Move dislocations
    /**
     * @brief Displace all the dislocations.
     * @param minDistance The minimum distance allowed between two defects.
     * @param dt The value of the time increment.
     * @param mu Shear modulus (Pa).
     * @param nu Poisson's ratio.
     */
    void moveAllDislocations (double minDistance, double dt, double mu, double nu);

    // Dipole emissions
    /**
     * @brief Check all the dislocation sources in all the grains for dislocation dipole emissions.
     * @param dt The time increment in this iteration.
     * @param mu Shear modulus (Pa).
     * @param nu Poisson's ratio.
     * @param minDistance The limiting distance of approach between two defects.
     */
    void checkDislocationSources (double dt, double mu, double nu, double minDistance);

    // Local reactions
    /**
     * @brief Check the local reactions between defects within all the grains.
     * @param reactionRadius The limiting distance between to defects for which a local reaction can take place.
     */
    void checkPolycrystalLocalReactions (double reactionRadius);

    // Statistics
    /**
     * @brief Write the positions of all defects to file.
     * @param fileName The name mask for the file into which the data will be written.
     * @param t The time instant.
     */
    void writeAllDefects (std::string fileName, double t);
};

#endif // POLYCRYSTAL_H
