/*
 *  LevelLoader.h
 *  Raindrops
 *
 *  Created by Deborah Schmidt.
 *  Copyright 2010 All rights reserved.
 *
 */
#ifndef _LEVELLOADER_H
#define _LEVELLOADER_H

#include <fstream>
#include <iostream>
#include <string>
 #include <vector>
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml.hpp"


using namespace rapidxml;
using namespace std;

class LevelLoader{


private:

    char                            xml_location;
    xml_document<>                  doc;
    xml_node<>*                     game;
    b2World*                        myworld;
    int                             *mywidth;
    int                             *myheight;

public:

    int                             level;
    int                             level_count;
    bool                            sound;
    vector< vector<int> >           color_branch;
    vector< vector<int> >           color_drop;
    vector< vector< vector<int> > > flower_prefs;
    vector<ofColor>                 color_background;
    vector<ofColor>                 color_line;
    vector<ofColor>                 color_water;
    vector<ofColor>                 color_fly;
    vector<string>                  bg_image;

    LevelLoader(){
        level = 0;
    }

    void setup(b2World *world, int *width, int *height, string xml_location = "../data/level.xml") {
        cout << "Setup";
        myworld = world;
        mywidth = width;
        myheight = height;

        string input_xml;


        string line;
        ifstream in(xml_location.c_str());
        while(getline(in,line))
            input_xml += line;

		/*
		 input_xml = "<game>";
		 input_xml += "<level id='1'>";
		 input_xml += "<branch><color>red</color></branch>";
		 input_xml += "<flower><item x='500' height='300' width='20' branches='1'/></flower>";
		 input_xml += "<drop><color>blue</color></drop>";
		 input_xml += "</level>";
		 input_xml += "<sound>on</sound>";
		 input_xml += "</game>";
		 */
        vector<char> xml_copy(input_xml.begin(), input_xml.end());
        xml_copy.push_back('\0');

        doc.parse<0>(&xml_copy[0]);
        //cout<<"\n XML FORST NODE IS :"<<doc.first_node()->name()<<"\n";

        if (doc.first_node() != NULL) {
            cout << "" << xml_location << " successfully loaded." << endl;
            game = doc.first_node("game");
            loadData();
        }
        else {
          //  printf("Failed to load file \"%s\"\n", xml_location);
			cout << "Failed to load file" <<endl;
        }

    }

    void loadData() {

        //cout << "levelloader::loaddata::sound" << endl;

        (*mywidth) = atoi(game->first_attribute("width")->value());
        (*myheight) = atoi(game->first_attribute("height")->value());

        //get sound preference
        string sound_str = game->first_node("sound")->value();
        sound = (sound_str.compare("on") == 0);

        level_count = atoi(game->last_node("level")->first_attribute("id")->value());

        for (int j = 1; j<=level_count; j++) {

            cout << "levelloader::loaddata::level" << j << "::current_node" << endl;

            //get right level node
            xml_node<>* level_node = game->first_node("level");
            string level_id = level_node->first_attribute("id")->value();
            while( (atoi(level_id.c_str()) != j) && level_node->next_sibling()!=NULL) {
                level_node = level_node->next_sibling();
                level_id = level_node->first_attribute("id")->value();
            }

            //cout << "levelloader::loaddata::level" << j << "::color_scheme1" << endl;

            //get color schemes

            vector<int> temp_color_branch;
            for(xml_node<>* i = level_node->first_node("branch")->first_node(); i!=NULL; i = i->next_sibling()) {
                temp_color_branch.push_back(getColorID(i->value()));
            }
            color_branch.push_back(temp_color_branch);

            //cout << "levelloader::loaddata::level" << j << "::color_scheme2" << endl;

            vector< vector<int> > temp_flower_prefs;
            for(xml_node<>* i = level_node->first_node("flower")->first_node(); i!=NULL; i = i->next_sibling()) {
                vector<int>temp(4);
                temp[0] = atoi(i->first_attribute("x")->value());
                temp[1] = atoi(i->first_attribute("width")->value());
                temp[2] = atoi(i->first_attribute("height")->value());
                temp[3] = atoi(i->first_attribute("branches")->value());
                temp_flower_prefs.push_back(temp);
            }
            flower_prefs.push_back(temp_flower_prefs);

            //cout << "levelloader::loaddata::level" << j << "::color_scheme3" << endl;

            vector<int> temp_color_drop;
            for(xml_node<>* i = level_node->first_node("drop")->first_node(); i!=NULL; i = i->next_sibling()) {
                temp_color_drop.push_back(getColorID(i->value()));
            }
            color_drop.push_back(temp_color_drop);

            ofColor temp_color_water;
            temp_color_water.r = atoi(level_node->first_node("water")->first_attribute("r")->value());
            temp_color_water.g = atoi(level_node->first_node("water")->first_attribute("g")->value());
            temp_color_water.b = atoi(level_node->first_node("water")->first_attribute("b")->value());
            color_water.push_back(temp_color_water);

            ofColor temp_color_line;
            temp_color_line.r = atoi(level_node->first_node("line")->first_attribute("r")->value());
            temp_color_line.g = atoi(level_node->first_node("line")->first_attribute("g")->value());
            temp_color_line.b = atoi(level_node->first_node("line")->first_attribute("b")->value());
            color_line.push_back(temp_color_line);

            ofColor temp_color_background;
            temp_color_background.r = atoi(level_node->first_node("background")->first_attribute("r")->value());
            temp_color_background.g = atoi(level_node->first_node("background")->first_attribute("g")->value());
            temp_color_background.b = atoi(level_node->first_node("background")->first_attribute("b")->value());
            color_background.push_back(temp_color_background);

            ofColor temp_color_fly;
            temp_color_fly.r = atoi(level_node->first_node("fly")->first_attribute("r")->value());
            temp_color_fly.g = atoi(level_node->first_node("fly")->first_attribute("g")->value());
            temp_color_fly.b = atoi(level_node->first_node("fly")->first_attribute("b")->value());
            color_fly.push_back(temp_color_fly);

            //cout << "levelloader::loaddata::level" << j << "::color_scheme4 " << endl;

            bg_image.push_back(level_node->first_node("background")->first_attribute("src")->value());

        }

        //cout << "levelloader::loaddata::end" << endl;

    }

    void nextLevel(vector<Plant> *plants, RaindropLine *line, Background *background, int *dropsDown, bool success) {

        if(success)
			level = (level>=level_count) ? 1 : level+1;

        cout << "level " << level << " loading.."<< endl;

        //cout << "levelloader::nextlevel::line->setup"<< endl;

        line->setup(myworld, dropsDown, *mywidth, *myheight, color_drop[level-1], color_background[level-1], color_line[level-1]);
        cout<<"trial 1 \n";
        background->setup(myworld, dropsDown, *mywidth, *myheight, color_water[level-1], color_background[level-1], color_fly[level-1], bg_image[level-1]);

        cout << "levelloader::nextlevel::plants->clear"<< endl;

        plants->clear();

        //cout << "levelloader::nextlevel::color_shuffle"<< endl;

        vector<int> temp;

        for(int i = 0; i<color_drop[level-1].size(); i++) {
            bool missing_color = true;
            for(int ii = 0; ii<temp.size();ii++) {
                if(color_drop[level-1][i] == temp[ii]) {
                    missing_color = false;
                    break;
                }
            }
            if(missing_color)
                temp.push_back(color_drop[level-1][i]);
        }

        random_shuffle(temp.begin(), temp.end());


        for(int i = 0; i<flower_prefs[level-1].size(); i++) {

            //cout << "levelloader::nextlevel::plant" << i << endl;

            Plant p;

            int color_id;
            if(i<temp.size())
                color_id = temp[i];
            else
                color_id = color_drop[level-1][(int)ofRandom(0,color_drop[level-1].size())];

            int random_index = ofRandom(0,color_branch[level-1].size());

            int temp_c_branch = color_branch[level-1][random_index];

            p.setup(myworld,*mywidth, flower_prefs[level-1][i][0], *myheight, flower_prefs[level-1][i][3], flower_prefs[level-1][i][2],
                    flower_prefs[level-1][i][1], color_id, temp_c_branch);

            plants->push_back(p);

        }

        cout << "level " << level << " loading done."<< endl;

    }

    int getColorID(string color) {

        int id = 0;

        if(color.compare("brown")==0)
            id=0;
        else if(color.compare("green")==0)
            id=1;
        else if(color.compare("red")==0)
            id=2;
        else if(color.compare("blue")==0)
            id=3;
        else if(color.compare("grey")==0)
            id=4;
        else
            id=5;

        return id;

    }




};


#endif

