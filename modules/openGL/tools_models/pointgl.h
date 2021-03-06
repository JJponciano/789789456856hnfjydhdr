/**
*  @copyright 2015 Jean-Jacques PONCIANO, Claire PRUDHOMME
* All rights reserved.
* This file is part of scn reader.
*
* scn reader is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* scn reader is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Foobar.  If not, see <http://www.gnu.org/licenses/>
* @author Jean-Jacques PONCIANO and Claire PRUDHOMME
* Contact: ponciano.jeanjacques@gmail.com
* @version 0.1
*/
#ifndef POINTGL_H
#define	POINTGL_H
#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <stdlib.h>
class PointGL {
public:
    /**
     * Construteur par défaut construisant le point à l'origine du repère
     */
    PointGL();
    /**
     * Construction du point
     * @param x abscisse
     * @param y ordonnée
     * @param z profondeur
     */
    PointGL(double x, double y,double z);
    PointGL(const PointGL& orig);
    bool operator==(PointGL const& a)const;
    bool operator<(const PointGL &r)const;
    bool operator!=(PointGL const& a);
    void operator-(PointGL const& a);
    bool distanceX(PointGL const point, double distance) const;
    virtual ~PointGL();
    /**
     * Récupération de l'abscisse du point
     * @return  abscisse du point
     */
    double getX()const;
    /**
     * Récupération de l'ordonnée du point
     * @return  ordonnée du point
     */
    double getY()const;
    /**
     * Récupération de la profondeur du point
     * @return  profondeur du point
     */
    double getZ()const;
    /**
     * Modification de l'abscisse du point
     * @param a nouvel abscisse
     */
    void setX(double a);
    /**
     * Modification de l'ordonné du point
     * @param a nouvel ordonné
     */
    void setY(double a);
    /**
     * Modification de la prodondeur du point
     * @param a nouvelle prodondeur
     */
    void setZ(double a);
    
    /**
     * Modifie la valeur de la normal moyenne  en ce point
     * @param n normale moyenne à affecter
     */
    void setNormalMoyenne(std::vector<double>n);
    /**
     * Modifie la valeur de la normal moyenne  en ce point
     * @param x
     * @param y
     * @param z
     */
    void setNormalMoyenne(double x, double y, double z);
    /**
     * Récupération du vecteur normal à la face
     * @return le vecteur normal
     */
    std::vector<double> getNormal()const;
    /**
     * Opération de translation
     * @param x
     * @param y
     * @param z
     */
    void translation(double x,double y,double z);
    /**
     * Opération de rotation
     * @param angle angle de rotation
     * @param x axe de rotation
     * @param y axe de rotation
     * @param z axe de rotation
     */
    void rotation(double a, int x,int y,int z);
    /**
     * Opération de mise à l'échelle
     * @param x
     * @param y
     * @param z
     */
    void scale(double x,double y,double z);

    /**
     * @brief truncation rounded to trunc th
     * @param trun significant figure guard, power of 10
     * @param f double to be truncated
     * @return the double truncated
     */
    double truncation(int trunc,double f);
    int getEpsilon() const;
    /**
     * @brief setEpsilon setter
     * @param value  Significant figure guard, power of 10(10 or 100 or 1000...)
     */
    void setEpsilon(int value);
    /**
     * @brief equals2D test if points are equals in 2D ( without z coordinate)
     * @param a point to be tested
     * @return true if points are similars
     */
    bool equals2D(const PointGL a) const;
    bool distanceY(const PointGL point, double distance) const;

private:
    double rad;
    double x;
    double y;
    double z;
    int epsilon;///< use for tested if double are equals. Significant figure guard, power of 10
    std::vector<double>normalMoyenne;
};

#endif	/* POINT_H */

