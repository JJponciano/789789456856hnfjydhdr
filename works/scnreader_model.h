
/**
* @copyright 2015 Jean-Jacques PONCIANO, Claire PRUDHOMME
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

#ifndef SCNREADER_MODEL_H
#define SCNREADER_MODEL_H


#include "../modules/exceptions/erreur.h"
#include "scndata.h"
#include "railcluster.h"
#include "listerail.h"
#include <QVector>
#include <QHash>
#include <QDataStream>

/**
 * @class scnreader_model
 * @brief The scnreader_model is a daughter class of ToolsPCL, so it manages too pcl.
 * This class is used to loading, reading and processing of point clouds.
 * It can be used as its mother but it ha some special functions to do treatments which are more particulary as ToolsPCL.
 * The aim of this class is to detect switchs which are in the cloud of points given by the file which are uploaded.
 *
 * @details
 *
 * \subsection{How to use}
 *
 *
 * \subsection{Load cloud}
 *
 * \subsubsection{From TXT file format}
 *  if you want load a cloud from a TXT file format:
 * @code
 *   std::string pathname="myfile.TXT";
 *   pcl::PointCloud<pcl::PointXYZ>::Ptr cloud=ToolsPCL::loadCloudFromTXT2(pathname.c_str());
 * @endcode
 *
 * \subsection{Processing}
 *
 * \subsubsection{Planar segmentation}
 * if you want to execute a planar segmentation on the interval of work 6400-6600:
 *  @code
 * std::string pathname="myfile.pcd";
 * pcl::PointCloud<pcl::PointXYZ>::Ptr cloud=ToolsPCL::loadCloud(pathname.c_str());
 * int d=6400;
 * int f=6600;
 * pcl::PointCloud<pcl::PointXYZ>::Ptr plan=this->planar_segmentation(d,f);
 * @endcode
 *
 * \subsection{Exemple}

*/

class scnreader_model: public ToolsPCL
{
public:
    scnreader_model();
    ~scnreader_model();

    void clear();

    /**
     * @brief scnreader_model::loadCloudFromTXT create and add data from file
     * @param pathname the file in txt format
     */
    void loadCloudFromTXT2(std::string pathname);

    /**
     * @brief addCloud load a cloud from a "txt" format file and add it in clouds
     * @param pathname path of the file for load cloud.
     */
    virtual void addCloudFromTXT(std::string pathname);


    /**
     * @brief scnreader_model::loadFromSCN create and add data from file
     * @param pathname the file in scn format
     */
    void  loadFromSCN(std::string pathname);


    /**
     * @brief scnreader_model::planar_segmentation segmentate a part of cloud
     * @param d is the first footpulse of the cloud's part and f is the last footpulse of the cloud's part
     */
    void planar_segmentation( int d, int f);

    /**
    * @brief acces to QHash
    * @return the Qhash which contains all points footpulse by footpulse
    */
    QHash <int, QVector<pcl::PointXYZ *> *> getNuage();

    /**
     * @brief acces to QHash
     * @return the Qhash which contains all points of segmentations of a cloud's part
     */
    QHash <QString, QVector<pcl::PointXYZ*>*> getSegmentation();

    /**
       * @brief createRail fills lesrails for all footpulse which were in the uploaded file
       */
    void createRail();

    /**
       * @brief getVectInCloud transform a vector of point in a cloud
       * @param vecteur is the vector which we will transform
       * @return the cloud corresponding to the vector
       */
    pcl::PointCloud<pcl::PointXYZ>::Ptr getVectInCloud(QVector<pcl::PointXYZ *> vecteur);
    /**
       * @brief getVectInCloud transform a vector of point in a cloud
       * @param vecteur is the vector which we will transform
       * @return the cloud corresponding to the vector
       */
    pcl::PointCloud<pcl::PointXYZ>::Ptr getVectInCloud(QVector<PointGL> vecteur);

    /**
       * @brief scnreader_model::getCloudInVect transform a cloud of point in a vector
       * @param cloud is the cloud which we will transform
       * @return the vector corresponding to the cloud
       */
    QVector<pcl::PointXYZ *> getCloudInVect(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);


    //Access in reading and writing of variables
    ListeRail getLesRails() const;

    ListeRail getLesRailsOptimize() const;
    void setLesRailsOptimize(const ListeRail &value);

    pcl::PointCloud<pcl::PointXYZ>::Ptr getResultRANSAC() const;
    void setResultRANSAC(const pcl::PointCloud<pcl::PointXYZ>::Ptr &value);

    QVector<PointGL> getCloudInVectpoint(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

    QVector<int> getLesSwitchs() const;
    void setLesSwitchs(const QVector<int> &value);

    QString getNomFile() const;
    void setNomFile(const QString &value);

    int getCapacity() const;
    void setCapacity(int c);

    int getFtpd();
    int getFtpf();

    void setFtpd(int d);
    void setFtpf(int f);

    ScnData getData(int i) const;
    void setDatas(const QVector<ScnData> &value);

    //      /**
    //       * @brief acces to QHash TODO OR NOT
    //       * @return the Qhash which contains all points of exractions of a cloud's part
    //       */
    //       QHash <QString, QVector<pcl::PointXYZ *>*> getExtraction();

    /**
        * @brief scnreader_model::extractionCloud extract a part of cloud TODO OR NOT
        * @param d is the first footpulse of the cloud's part and f is the last footpulse of the cloud's part
        */
    void extractionCloud(int d, int f);

    bool getRansacVide() const;
    void setRansacVide(bool value);
private:
    int capacity;
    int workWindows;
    QVector <ScnData> datas;
    int ftpd;
    int ftpf;
    ListeRail lesRails;
    ListeRail lesRailsOptimize;
    pcl::PointCloud<pcl::PointXYZ>::Ptr resultRANSAC;
    QVector <int>LesSwitchs;///< list of the footpulse corresponding to switchs
    QString nomFile;
    bool RansacVide;
    bool cfs;
    /**
       * @brief samePoint watch if two points are the same or not
       * @param ptP the point which is contained in QHash
       * @param pt the point which is contained in cloud
       * @return if they are the same
       */
    bool samePoint( pcl::PointXYZ* point2, pcl::PointXYZ *ptP);
    /**
     * @brief readData read a data byte by byte
     * @param bytePosition posision of the data
     * @param length length of bytes of the data
     * @param pathname path of file data
     * @return the data to QString
     */
    QString readData(int bytePosition, int length, std::string pathname, bool isnumber);

    /**
     * @brief loadCloudFromTXT load a cloud from a "txt" format file
     * @param pathname path of the file for load cloud.
     */
    void loadCloudFromTXT(std::string pathname);

    /**
     * @brief IsFootpulse find what axe is the footpulse
     * @param pathname the file in txt format
     * @return int which is the number corresponding of axe (0=x, 1=y, 2=z)
     */
    int IsFootpulse(std::string pathname);

    /**
     * @brief scnreader_model::getPartInCloud transform the part of works in a cloud
     * @param d is the footpulse which begin the part
     * @param f is the footpulse which finish the part
     * @return the cloud corresponding to the part of cloud which we work on
     */
    pcl::PointCloud<pcl::PointXYZ>::Ptr getPartInCloud(int d, int f, QVector<int>* tailles);

    /**
     * @brief getCloudInVect transform a cloud in a vector of points
     * @param cloudTemp is the cloud which we will transform
     * @return the vector of points corresponding
     */
    QVector<pcl::PointXYZ *> * getCloudInVect2(pcl::PointCloud<pcl::PointXYZ>::Ptr cloudTemp);

    /**
     * @brief scnreader_model::getPtWithInd take the points correponding to indices which are given
     * @param d is the footpulse which begin the part
     * @param f is the footpulse which finish the part
     * @param ind is the vector of indices
     * @return the vector of points corresponding to indices
     */
    QVector<pcl::PointXYZ*>* getPtWithInd(int d, int f, std::vector<int> indices, QVector<int>* tailles);

    /**
     * @brief the Qhash which contains all points footpulse by footpulse
     *
     */
    QHash <int, QVector<pcl::PointXYZ *> *> nuage;
    /**
     * @brief the Qhash which contains all points of segmentations of a cloud's part
     *
     */
    QHash <QString, QVector<pcl::PointXYZ*>*> segmentation;
    /**
      * @brief the Qhash which contains all points of extractions of a cloud's part
      *
      */
    QHash <QString, QVector<pcl::PointXYZ *>*> extraction;
    /**
      * @brief lesRails contains tracks for each footpulse
      *
      */
    void optimization();
    void enregistre(QString noms);
    void VideEtEnregistre(QString noms);

};

#endif // SCNREADER_MODULE_H
