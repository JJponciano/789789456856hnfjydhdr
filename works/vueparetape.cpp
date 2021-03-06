
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
#include "vueparetape.h"

VueParEtape::VueParEtape(QWidget *parent): groundGLWidget(parent)
{

    //this->ftpD=0;
    //this->ftpF=0;
    this->ftpDI=0;
    this->ftpFI=0;
    this->px=0;
    this->py=0;
    this->pz=0;
    this->affs=false;
    this->affswitch=false;
    this->affe=false;
    this->affr=false;
    this->affReg=false;
    this->affc=true;
    this->mirx=0;
    this->numS=-1;
    this->posSwitch=0;
}

VueParEtape::~VueParEtape()
{

}


// ------------------------------------------ OPENGL Functions ------------------------------------------
void VueParEtape::initializeGL()
{
    groundGLWidget::initializeGL();
    glDisable(GL_LIGHTING);
}

void VueParEtape::resizeGL(int width, int height)
{
    groundGLWidget::resizeGL( width,  height);
}

void VueParEtape::paintGL()
{
    //call the superclass function
    groundGLWidget::paintGL();



    glPointSize(1);
    glPushMatrix();

    if(affc)
    {
        this->affichageCloud();
    }
    if(affswitch)
    {
        this->affichageSwitch();
    }
    if(affs)
    {
        this->affichageSegm();
    }
    if(this->affReg)
    {
        this->affichageReg();
        //this->affichageIm();
    }
    if(affe)
    {
        QVector <PointGL> rails=this->scnreaderFond.getLesRails().getCloud();
        if(rails.size()>0)
        {
            glBegin(GL_POINTS);
            for(int i=0; i<rails.size(); i++)
            {
                glColor3f(0.0,1.0,1.0);
                glVertex3f(rails.at(i).getX(), rails.at(i).getY(), (rails.at(i).getZ()-this->ftpDI)*0.1);
            }
            glEnd();
        }
    }

    if(affr)
    {
        pcl::PointCloud<pcl::PointXYZ>::Ptr resultRANSAC=this->scnreaderFond.getResultRANSAC();

        if(!this->scnreaderFond.getRansacVide())
        {
            QVector <int> switchs= scnreaderFond.getLesRailsOptimize().getSwitchDetected();
            glBegin(GL_POINTS);

            for(int i=0; i<resultRANSAC->points.size(); i++)
            {
                int z=resultRANSAC->points.at(i).z;
                if(switchs.contains(z))
                    glColor3f(1.0,1.0,0.0);
                else
                    glColor3f(1.0,0.0,1.0);
                glVertex3f(resultRANSAC->points.at(i).x, resultRANSAC->points.at(i).y, (z-this->ftpDI)*0.1);

            }
            glEnd();
        }
    }
    glPopMatrix();
    //----------------------------------------------------------------------*/
}

// ------------------------------------------ Action Functions ------------------------------------------
void VueParEtape::affichageSwitch()
{
    if(!AucunSwitch())
    {
        int deb= this->numS-50;
        if(deb<this->scnreaderFond.getFtpd())
            deb=this->scnreaderFond.getFtpd();
        int fin= this->numS+50;
        if(fin>this->scnreaderFond.getFtpf())
            fin=this->scnreaderFond.getFtpf();

        glBegin(GL_POINTS);
        for(int j=deb; j<=fin; j++)
        {
            if(this->scnreaderFond.getNuage().contains(j))
            {
                QVector <PointGL*>* v=this->scnreaderFond.getNuage().value(j);
                if(j==this->numS)
                    glColor3f(1.0,0.0,0.0);
                else
                    glColor3f(1.0,1.0,1.0);

                for(int i=0; i<v->size(); i++)
                {
                    glPointSize(1);
                    //keep coordinates of points to draw them
                    double x=(* (v->at(i))).getX();
                    double y=(* (v->at(i))).getY();
                    double z=((* (v->at(i))).getZ()-this->numS)*0.1;
                    glVertex3f(x,y,z);
                }

            }
        }
        glEnd();
    }
}
void VueParEtape::affichageIm()
{
    glBegin(GL_POINTS);
    int w=this->scnreaderFond.getIm().getWidth();
    int h=this->scnreaderFond.getIm().getHeight();
    if(w!=0 && h!=0)
    {
        for(int i=0; i<h; i++)
        {
                for(int j=0; j<w; j++){
                    if(this->scnreaderFond.getIm().getValue(i,j)>0)
                    {
                        QVector<double> coul=colors.getColor(this->scnreaderFond.getIm().getValue(i,j));
                        glColor3f(coul.at(0),coul.at(1),coul.at(2));
                        glPointSize(1);
                        //normalizes points with model->max of cordinates previously finded
                        double x=j*0.1-(0.1*w/2.0);
                        double y=0;
                        double z=i*0.1;
                        glVertex3f(x,y,z);
                    }
                }
        }
    }
    glEnd();
}

void VueParEtape::affichageReg()
{
    glBegin(GL_POINTS);
    QVector< QVector<PointGL> >regions=this->scnreaderFond.getRegions();

    for(int i=0; i<regions.size(); i++)
    {
        glColor3f(colors.getColor(i).at(0),colors.getColor(i).at(1),colors.getColor(i).at(2));

        for(int j=0; j<regions.at(i).size(); j++){
            glPointSize(1);
            //normalizes points with model->max of cordinates previously finded
            double x=(regions.at(i).at(j)).getX();
            double y=(regions.at(i).at(j)).getY();
            double z=((regions.at(i).at(j)).getZ()-this->ftpDI)*0.1;
            glVertex3f(x,y,z);
        }
    }
    glEnd();
}
void VueParEtape::affichageCloud()
{
    glBegin(GL_POINTS);
    int j=this->ftpDI;
    bool stop=false;
    QVector <int> switchs= scnreaderFond.getLesSwitchs();
    while( !stop && j<=this->ftpFI)
    {
        if(this->scnreaderFond.getNuage().contains(j))
        {

            QVector <PointGL*>* v=this->scnreaderFond.getNuage().value(j);
            for(int i=0; i<v->size(); i++)
            {
                glPointSize(1);

                //normalizes points with model->max of cordinates previously finded
                double x=(* (v->at(i))).getX();//scnreaderFond.getMaxX()*10;
                double y=(* (v->at(i))).getY();//scnreaderFond.getMaxY()*10;
                double z=((* (v->at(i))).getZ()-this->ftpDI)*0.1;//scnreaderFond.getMaxZ()*10;

                //                                glVertex3f(x,int(y*1000)/100,z);
                if(SwitchContenu((* (v->at(i))).getZ()))
                    glColor3f(1.0,0.0,0.0);
                else
                    glColor3f(1.0,1.0,1.0);
                glVertex3f(x,y,z);
            }

        }
        else
            stop=true;

        j++;
    }
    glEnd();
}
void VueParEtape::affichageSegm()
{
    std::stringstream ss;
    ss << "S_" << this->ftpDI <<"_" << this->ftpFI;
    QString chaine=QString::fromStdString (ss.str());

    QHash<QString, QVector<PointGL*>*> h=this->scnreaderFond.getSegmentation();

    if(!h.contains(chaine))
    {
        try{
            this->scnreaderFond.planar_segmentation(this->ftpDI, this->ftpFI);
        }catch(std::exception const& e){
            QMessageBox::critical(0, "Error", e.what());
        }
    }
    QVector <PointGL*>* vect=this->scnreaderFond.getSegmentation().value(QString::fromStdString (ss.str()));
    glBegin(GL_POINTS);
    for(int j=0;j<vect->size();j++)
    {
        glColor3f(0.0,0.0,1.0);
        glVertex3f((* (vect->at(j))).getX(),(*(vect->at(j))).getY(),((* (vect->at(j))).getZ()-this->ftpDI)*0.1);
    }
    glEnd();

}
bool VueParEtape::getAffswitch() const
{
    return affswitch;
}

void VueParEtape::setAffswitch(bool value)
{
    affswitch = value;
}


void VueParEtape::loadCloudFromTXT(){
    try{
        //Opens a window allowing the user to select the file to load
        QString fileName = QFileDialog::getOpenFileName(NULL,"Load", "","Load a cloud (*.txt);;All Files (*)");

        // if user have seleted a directory
        if (!fileName.isEmpty())
        {

            this->nomFichier=fileName.toStdString();
            QString nom=this->KeepName(fileName);
            this->scnreaderFond.setNomFile(nom);

            this->scnreaderFond.loadCloudFromTXT2(fileName.toStdString());

            if(this->scnreaderFond.getNuage().size() > 0)
            {
                this->ftpDI=this->scnreaderFond.getFtpd();
                this->ftpFI=this->scnreaderFond.getFtpd();
                QVector <PointGL > rails=this->scnreaderFond.getLesRails().getCloud();
                /*  for(int i=0;i<rails.size();i++){
                    if( rails[i]->x<0)
                              rails[i]->y+=1;
                }*/
            }
            else throw Erreur("Le fichier ne contient pas de point");
            this->LectureSw(nom);
            //this->ftpdeDepart=this->scnreaderFond.getClouds(0)->points[0].z;
            //this->ftpCourant=this->scnreaderFond.getClouds(0)->points[0].z;
        }

    }catch(std::exception const& e){
        QMessageBox::critical(0, "Error", e.what());
    }

}
void VueParEtape::loadCloud(){
    try{
        //Opens a window allowing the user to select the file to load
        QString fileName = QFileDialog::getOpenFileName(0,"Load", "","Load a cloud (*.pcd);;All Files (*)");

        // if user have seleted a directory
        if (!fileName.isEmpty())
        {
            this->scnreaderFond.addCloud(fileName.toStdString());
            this->nomFichier=fileName.toStdString();

            if(this->scnreaderFond.getNuage().size() > 0)
            {
                this->ftpDI=this->scnreaderFond.getFtpd();
                this->ftpFI=this->scnreaderFond.getFtpd();
            }
            else throw Erreur("Le fichier ne contient pas de point");
            // this->ftpdeDepart=this->scnreaderFond.getClouds(0)->points[0].z;
            //this->ftpCourant=this->scnreaderFond.getClouds(0)->points[0].z;
        }

    }catch(std::exception const& e){
        QMessageBox::critical(0, "Error", e.what());
    }
}
void VueParEtape::saveClouds(){
    try{
        //Opens a window allowing the user to select the file to save
        QString fileName = QFileDialog::getSaveFileName(0,"Save", "","save all clouds (*.pcd);;All Files (*)");

        // si l'utilisateur a sélectionné un nom
        if (!fileName.isEmpty())
        {
            this->scnreaderFond.saveClouds(fileName.toStdString());
        }
    }catch(std::exception const& e){
        QMessageBox::critical(0, "Error", e.what());
    }
}
void VueParEtape::saveCloudsFromTXT(){
    try{
        //Opens a window allowing the user to select the file to save
        QString fileName = QFileDialog::getSaveFileName(0,"Save", "","save all clouds (*.txt);;All Files (*)");

        // si l'utilisateur a sélectionné un nom
        if (!fileName.isEmpty())
        {
            //this->scnreaderFond.saveCloudsFromTXT(fileName.toStdString());
            this->scnreaderFond.SavePartInTxt(this->ftpDI, this->ftpFI, fileName);

        }
    }catch(std::exception const& e){
        QMessageBox::critical(0, "Error", e.what());
    }
}

void VueParEtape::loadFromSCN(){
    try{
        //Opens a window allowing the user to select the file to load
        QString fileName = QFileDialog::getOpenFileName(0,"Read", "","Read file (*.scn);;All Files (*)");

        // if user have seleted a directory
        if (!fileName.isEmpty())
        {
            QString nom=this->KeepName(fileName);
            this->scnreaderFond.setNomFile(nom);
            std::cout<<"start"<<std::endl;
            this->scnreaderFond.loadFromSCN(fileName.toStdString());
            std::cout<<"end"<<std::endl;
            this->nomFichier=fileName.toStdString();
            if(this->scnreaderFond.getNuage().size() > 0)
            {
                this->ftpDI=this->scnreaderFond.getFtpd();
                this->ftpFI=this->scnreaderFond.getFtpd();
            }
            else throw Erreur("Le fichier ne contient pas de point");
        }
    }catch(std::exception const& e){
        QMessageBox::critical(this, "Error", e.what());
    }
}

void VueParEtape::clear(){
    this->scnreaderFond.clear();
    this->SwitchDetected.clear();
    this->affs=false;
    this->affswitch=false;
    this->affe=false;
    this->affr=false;
    this->affc=true;
    this->mirx=0;
    this->numS=-1;
    this->posSwitch=0;
}
void VueParEtape::planarSegmentation(int d, int f){
    try{
        this->scnreaderFond.planar_segmentation( d, f);
    }catch(std::exception const& e){
        QMessageBox::critical(this, "Error", e.what());
    }
}

//===============================================
//              Clavier et souris
//===============================================
void VueParEtape::keyPressEvent(QKeyEvent *keyEvent)
{

    if(keyEvent->key()==Qt::Key_M){
        this->mirx-=0.01;
        std::cout<<this->mirx<<std::endl;
    }
    else
        if(keyEvent->key()==Qt::Key_N){
            this->mirx+=0.01;
            std::cout<<this->mirx<<std::endl;
        }
        else
            if(keyEvent->key()==Qt::Key_W){
                this->pZ++;
                this->lZ++;
            }
            else if(keyEvent->key()==Qt::Key_S){
                this->pZ--;
                this->lZ--;
            }
            else if(keyEvent->key()==Qt::Key_D){
                this->pX--;
                this->lX--;
            }
            else if(keyEvent->key()==Qt::Key_A){
                this->pX++;
                this->lX++;
            }
            else if(keyEvent->key()==Qt::Key_Q){
                this->pY++;
                this->lY++;
            }
            else if(keyEvent->key()==Qt::Key_E){
                this->pY--;
                this->lY--;
            }
            else if(keyEvent->key()==Qt::Key_8){
                this->lY++;
            }
            else if(keyEvent->key()==Qt::Key_5){
                this->lY--;
            }
            else groundGLWidget::keyPressEvent(keyEvent);
}

int VueParEtape::getFtpD(){
    return this->scnreaderFond.getFtpd();
}

int VueParEtape::getFtpF(){
    return this->scnreaderFond.getFtpf();
}

void VueParEtape::setFtpDI(int di){
    this->ftpDI=di;
}

void VueParEtape::setFtpFI(int fi){
    this->ftpFI=fi;
}

int VueParEtape::getFtpDI(){
    return this->ftpDI;
}

int VueParEtape::getFtpFI(){
    return this->ftpFI;
}

std::string VueParEtape::getNomF(){
    return this->nomFichier;
}

void VueParEtape::setaffC(bool b){
    this->affc=b;
}
void VueParEtape::setaffReg(bool b){
    this->affReg=b;
}

void VueParEtape::setaffS(bool b){
    this->affs=b;
}

void VueParEtape::setaffE(bool b){
    this->affe=b;
}

void VueParEtape::setaffR(bool b){
    this->affr=b;
}

int VueParEtape::getTaille(){
    return this->scnreaderFond.getNuage().size();
}

void VueParEtape::mouseMoveEvent(QMouseEvent *event){
    View_ground_GL::mouseMoveEvent(event);
}
void VueParEtape::mousePressEvent(QMouseEvent *event){
    View_ground_GL::mousePressEvent(event);
}
void VueParEtape::mouseReleaseEvent(QMouseEvent *event){
    View_ground_GL::mouseReleaseEvent(event);
}
QString VueParEtape::KeepName(QString fileName)
{
    QStringList result =fileName.split("/");
    QString n=result.at(result.size()-1);
    result =n.split(".");
    n="";
    for(int i=0; i<result.size()-1; i++)
        n.push_back(result.at(i));
    return n;
}
int VueParEtape::getPosSwitch() const
{
    return posSwitch;
}

void VueParEtape::IncreasePosSwitch()
{
    if(this->posSwitch<sizeAllSwitch()-1)
    {
        posSwitch ++;
        calculNumWithPos();
    }
}

void VueParEtape::DecreasePosSwitch()
{
    if(this->posSwitch>0)
    {
        posSwitch --;
        calculNumWithPos();
    }
}

int VueParEtape::getNumS()
{
    if(AucunSwitch())
        this->numS=-1;
    return numS;
}

void VueParEtape::setNumS(int value)
{
    numS = value;
}

void VueParEtape::calculNumWithPos()
{
    if(!this->SwitchDetected.isEmpty())
    {
        int pos=this->posSwitch;
        for(int i=0; i<this->SwitchDetected.size(); i++)
        {
            if(pos<this->SwitchDetected.at(i).size())
                this->numS=this->SwitchDetected.at(i).at(pos);
            else
                pos=pos-this->SwitchDetected.at(i).size();
        }
    }
}

int VueParEtape::sizeAllSwitch()
{
    if(this->SwitchDetected.isEmpty())
    {
        return 0;
    }
    else
    {
        int nbs=0;
        for(int i=0; i<this->SwitchDetected.size(); i++)
            nbs+=this->SwitchDetected.at(i).size();
        return nbs;
    }
}
bool VueParEtape::AucunSwitch()
{
    if(this->SwitchDetected.isEmpty())
    {
        return true;
    }
    else
    {
        int nbs=0;
        int i=0;
        while(nbs==0 && i<this->SwitchDetected.size())
        {
            nbs+=this->SwitchDetected.at(i).size();
            i++;
        }
        return (nbs==0);
    }
}

bool VueParEtape::SwitchContenu(int ftp)
{
    if(AucunSwitch())
    {
        return false;
    }
    else
    {
        int i=0;
        bool contenu=false;
        while(i<this->SwitchDetected.size() && !contenu)
        {
            if(this->SwitchDetected.at(i).contains(ftp))
                contenu=true;
            i++;
        }
        return contenu;
    }
}

void VueParEtape::LectureSw(QString nameF)
{
    QString noms=nameF;
    noms.push_back("_switch.txt");
    QFile fichier(noms);
    if(fichier.exists())
    {
        if(fichier.open(QIODevice::ReadOnly | QIODevice::Text)){
            //if you can, initialize flu and line
            QTextStream flux(&fichier);
            QString ligne;
            QVector<int> vect;
            while(!flux.atEnd())
            {
                ligne= flux.readLine();
                vect.push_back(ligne.toInt());


                if(vect.size()>=this->scnreaderFond.getCapacity()-1)
                {
                    this->SwitchDetected.push_back(vect);
                    QVector<int> vect2;
                    vect=vect2;
                }

            }
            this->SwitchDetected.push_back(vect);
            fichier.close();
            if(!this->SwitchDetected.isEmpty())
                this->numS=this->SwitchDetected.at(0).at(0);
        }
        else throw Erreur(" The file of switch can not be openned, check the write permission!");
    }
}
