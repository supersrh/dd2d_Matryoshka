/**
 * @file simulateSingleSlipPlane.cpp
 * @author Adhish Majumdar
 * @version 1.0
 * @date 10/06/2013
 * @brief Defintion various functions to simulate dislocation motion on a single slip plane.
 * @details This file defines various functions to simulate dislocation motion on a single slip plane. It also writes statistics to files which will be treated separately.
 */

#include "simulateSingleSlipPlane.h"

/**
 * @brief This function manages the simulation for a single slip plane.
 * @details This function is the point of entry into a simulation with a single slip plane.
 */
void simulateSingleSlipPlane ()
{
  std::string fName;

  std::cout << "Parameter file name: ";
  std::cin >> fName;

  SlipPlane *slipPlane;

  Parameter *param = new Parameter;

  if ( param->getParameters( fName ) )
  {
      slipPlane = new SlipPlane;
      if ( readSlipPlane ( param->dislocationStructureFile,  slipPlane ) )
      {
          //
      }
      else
      {
          delete ( slipPlane );
          slipPlane = NULL;

          delete ( param );
          param = NULL;

          return;
      }
  }
  else
  {
      delete ( param );
      param = NULL;

      return;
  }
}

/**
 * @brief Reads the slip plane details from the file.
 * @details The details of the slip plane and its dislocations are stored in a file the name of which is provided. This file is read and the information is saved into the instance of the SlipPlane class, the pointer to which is given.
 * @param fileName String containing the name of the file.
 * @param s Pointer to the instance of SlipPlane into which all data is to be stored.
 * @return Flag indicating the success or failure of the operation.
 */
bool readSlipPlane (std::string fileName, SlipPlane *s)
{
    std::ifstream fp ( fileName );
    std::string line;

    Vector3d *e;
    int i,  n;


    if ( fp.is_open() )
    {
        // Read the extremities
        e = new Vector3d[2];
        getline ( fp, line );
        e[0] = readVectorFromline ( line );
        getline ( fp, line );
        e[1] = readVectorFromLine ( line );
        s->setExtremities( e );
        delete ( e );
        e = NULL;

        // Read the normal vector
        getline ( fp, line );
        s->setNormal( readVectorFromLine ( line ) );

        // Read the position
        getline ( fp, line );
        s->setPosition( readVectorFromLine ( line ) );

        // Read number of dislocations
        fp >> n;
        // Read the dislocations
        for ( i=0; i<n; i++ ) {
            getline ( fp, line );
            s->insertDislocation ( readDislocationFromLine ( line ) );
        }

        // Read number of dislocation sources
        fp >> n;
        // Read the dislocation sources
        for ( i=0; i<n; i++ ) {
            getline ( fp,  line );
            s->insertDislocationSource ( readDislocationSourceFromLine ( line ) );
        }

        fp.close();
    }
    else
    {
        return ( false );
    }
}

/**
 * @brief Reads 3 values from a string and returns them in a Vector3d.
 * @param s The string that is to be read from.
 * @return Vector3d object containing the 3 values read.
 */
Vector3d readVectorFromLine (std::string s)
{
    std::stringstream ss ( s );
    std::string a;
    int i;
    Vector3d v;

    for ( i=0; i<3; i++ )
    {
        ss >> a;
        v.setValue( i, atof( a.c_str() ) );
    }

    return ( v );
}

/**
 * @brief Reads the data from a line and builds a dislocation from it.
 * @param s The string that is to be read from.
 * @return The dislocation.
 */
Dislocation readDislocationFromLine (std::string s)
{
    std::stringstresm ss ( s );
    std::string a;
    Vector3d pos, bvec, lvec;
    double bmag;
    bool mob;

    int i;

    // Read position
    for ( i=0; i<3; i++ ) {
        ss >> a;
        pos.setValue ( i, atof ( a.c_str() ) );
    }

    // Read Burgers vector
    for ( i=0; i<3; i++ ) {
        ss >> a;
        bvec.setValue ( i, atof ( a.c_str() ) );
    }

    // Read line vector
    for ( i=0; i<3; i++ ) {
        ss >> a;
        lvec.setValue ( i, atof ( a.c_str() ) );
    }

    // Read Burgers vector magnitude
    ss >> a;
    bmag = atof ( a.c_str() );

    // Read mobility
    ss >> a;
    mob = ( bool ) atoi ( a.c_str() );

    return ( Dislocation ( bvec, lvec, pos, bmag, mon ) );
}

/**
 * @brief Reads the data from a line and builds a dislocation source from it.
 * @param s The string that is to be read from.
 * @return The dislocation source.
 */
DislocationSource readDislocationSourceFromLine (std::string s)
{
    std::stringstresm ss ( s );
    std::string a;
    Vector3d pos, bvec, lvec;
    double bmag;
    double tau;
    in nIter;

    int i;

    // Read position
    for ( i=0; i<3; i++ ) {
        ss >> a;
        pos.setValue ( i, atof ( a.c_str() ) );
    }

    // Read Burgers vector
    for ( i=0; i<3; i++ ) {
        ss >> a;
        bvec.setValue ( i, atof ( a.c_str() ) );
    }

    // Read line vector
    for ( i=0; i<3; i++ ) {
        ss >> a;
        lvec.setValue ( i, atof ( a.c_str() ) );
    }

    // Read Burgers vector magnitude
    ss >> a;
    bmag = atof ( a.c_str() );

    // Read critical stress
    ss >> a;
    tau = atof ( a.c_str() );

    // Read number of iterations
    ss >> a;
    nIter = atoi ( a.c_str() );

    return ( Dislocation ( bvec, lvec, pos, bmag, tau, nIter ) );
}
