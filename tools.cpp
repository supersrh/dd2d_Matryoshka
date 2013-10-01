/**
 * @file tools.cpp
 * @author Adhish Majumdar
 * @version 1.0
 * @date 10/09/2013
 * @brief Definition various tools.
 * @details This file defines various tools that may come in handy.
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


#include "tools.h"

/**
 * @brief Creates all possible permutations of vectors having the same elements.
 * @details This function returns a vector container with all possible permutations of the present vector. This is useful in creating a list of equivalent vectors when the Miller indices of one are provided. For example, if the current vector is [1 1 0], the return vector container would contain the vectors [1 1 0], [1 0 1], [0 1 1], [-1 1 0], etc.
 * @return STL vector container with Vector3d variables created from permutations of the present vector elements.
 */
std::vector<Vector3d> permuteVector (Vector3d v)
{
    std::vector<Vector3d> vList;
    std::vector<Vector3d> vTemp;

    for (int i=0; i<3; i++)
    {
        vTemp = permuteVector_byIndex (v, i);
        vList = concatenateVectors (vList, vTemp);
    }

    return ( eliminateDuplicatesFromVector (vList, true) );
}

/**
 * @brief The vector is permuted keeping the index position constant.
 * @details This function permutes the given vector v keeping the element from the index position first. For example, if the vector is [a b c], and the index is 0, the permutations will contain [a b c], [-a b c], [a -b c], [a b -c], [a c b], [-a c b], [a -c b], [a c -b]. The permutations with sign changes are carried out by the function std::vector<Vector3d> permuteVector_signs (Vector3d v).
 * @param v The vector that has to be permuted.
 * @param index The index of the element of the vector v that must remain in position.
 * @return An STL vector container with the list of the permuted vectors keeping the indicated element in its original position.
 */
std::vector<Vector3d> permuteVector_byIndex (Vector3d v, int index)
{
    // The final vector that will be returned
    std::vector<Vector3d> vList;

    // The two possible vector lists
    std::vector<Vector3d> vList1;
    std::vector<Vector3d> vList2;

    Vector3d v1 = Vector3d (v.getValue(index),
                            v.getValue((index+1)%3),
                            v.getValue((index+2)%3));
    Vector3d v2 = Vector3d (v.getValue(index),
                            v.getValue((index+2)%3),
                            v.getValue((index+1)%3));

    // Create the permutations with the signs
    vList1 = permuteVector_bySign (v1);
    vList2 = permuteVector_bySign (v2);

    // Concatenate the two vectors
    vList = concatenateVectors (vList1, vList2);

    return (vList);
}

/**
 * @brief Permutations of the given vector by only changing signs of the elements.
 * @details This function permutes the vector by only changing the signs but not the positions the elements. For example, if the vector is [a b c], the permutations will contain [a b c], [-a b c], [a -b c], [a b -c].
 * @param v The vector that has to be permuted.
 * @return An STL vector container with the permuted vectors.
 */
std::vector<Vector3d> permuteVector_bySign (Vector3d v)
{
    // The list that will contain the permuted vectors.
    std::vector<Vector3d> vList (1, v);
    Vector3d vTemp;  // A temporary vector

    for (int i=0; i<3; i++)
    {
        vTemp = v;
        // Switch the sign of the i^th
        vTemp.setValue (i, (-1.0*vTemp.getValue(i)));
        vList.push_back (vTemp);
    }

    return (vList);
}

/**
 * @brief Concatenates two STL vectors containing elements of type Vector3d.
 * @param v1 First vector.
 * @param v2 Secone vector.
 * @return Concatenated vector.
 */
std::vector<Vector3d> concatenateVectors (std::vector<Vector3d> v1, std::vector<Vector3d> v2)
{
    v1.insert (v1.end(), v2.begin(), v2.end());

    return (v1);
}

/**
 * @brief Eliminates the duplicate vectors from the given STL vector.
 * @details This function eliminates duplicate vectors from the STL vector provided as argument and returns the result. The argument negatives is a flag to indicate if the negative of a vector is to be considered as its duplicate or not.
 * @param v The vector from which duplicates are to be removed.
 * @param negatives Flag to indicate if the negative of a vector should be considered as its duplicate or not.
 * @return The vector without duplicates.
 */
std::vector<Vector3d> eliminateDuplicatesFromVector (std::vector<Vector3d> v, bool negatives)
{
    std::vector<Vector3d>::iterator vi;
    std::vector<Vector3d>::iterator vj;

    bool found;

    std::vector<Vector3d> vList;
    Vector3d vPlus, vMinus;

    for (vi=v.begin();vi!=v.end();vi++)
    {
        for (vj=vi+1; vj!=v.end(); vj++)
        {
            vMinus = (*vi) - (*vj);

            found = (vMinus.magnitude()==0.0);
            if (negatives)
            {
                vPlus  = (*vi) + (*vj);
                found = found || (vPlus.magnitude()==0.0);
            }

            if (found)
            {
                break;
            }
        }
        if (!found)
        {
            vList.push_back(*vi);
        }
    }

    return (vList);
}

/**
 * @brief Checks if the input line is to be ignored or not.
 * @details A line in an input file may be empty or may be a comment. In these cases it should be ignored. This function tests if the given line is empty or begins with the character that indicates that it is a comment (default is #).
 * @param line The line that is to be tested.
 * @param comment The character that indicates a comment, default value is #.
 * @return A boolean flag saying if the line should be ignored or not.
 */
bool ignoreLine (std::string line, char comment)
{
    if ( line.empty () ) {
        return ( true );
    }

    return ( line.at ( 0 ) == comment );
}

/**
 * @brief Displays the message on screen.
 * @param message An std""string containing the message to be displayed.
 */
void displayMessage (std::string message)
{
    std::cout << std::endl << message << std::endl;
}

/**
 * @brief Converts an integer to an std::string.
 * @details This function uses ostringstream to convert an integer to a string.
 * @param i The integer to be converted to a string.
 * @return The integer converted to a string.
 */
std::string intToString (int i)
{
    std::ostringstream ss;
    ss << i;
    return ( ss.str() );
}

/**
 * @brief Converts an double to an std::string.
 * @details This function uses ostringstream to convert an double to a string.
 * @param i The double to be converted to a string.
 * @return The double converted to a string.
 */
std::string doubleToString (double i)
{
    std::ostringstream ss;
    ss << i;
    return ( ss.str() );
}

/**
 * @brief Function to get a vector container filled with a Gaussian distribution of doubles with the given mean and standard deviation.
 * @param n Number of doubles required.
 * @param mean The mean value of the Gaussian distribution.
 * @param stdev The standard deviation of the Gaussian distribution.
 * @return Vector container with the doubles in a Gaussian distribution with the required mean and standard deviation.
 */
std::vector<double> rng_Gaussian (int n, double mean, double stdev)
{
    // Prepare the random number generator
    gsl_rng* r;
    const gsl_rng_type* T;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    // Allocate the vector container
    std::vector<double> randoms(n);

    while (n > 0) {
        randoms[--n] = mean + gsl_ran_gaussian(r, stdev);
    }

    return (randoms);

}
