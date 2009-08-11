/* === S Y N F I G ========================================================= */
/*!	\file svg_parser.h
**	\brief Implementation of the Svg parser
**	\brief Based on SVG XML specification 1.1
**	\brief See: http://www.w3.org/TR/xml11/ for deatils
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2007, 2008 Chris Moore
**	Copyright (c) 2009 Carlos A. Sosa Navarro
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === S T A R T =========================================================== */

#ifndef __SVG_PARSER_H
#define __SVG_PARSER_H

/* === H E A D E R S ======================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <libxml++/libxml++.h>
#include <ETL/angle>
#include <ETL/stringf>
#include <synfig/canvas.h>
#include <synfig/loadcanvas.h>
#include <synfig/color.h>
#include <synfig/gamma.h>
#include <synfig/guid.h>

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig{

typedef struct matrix_t{
	float a,c,e;
	float b,d,f;
}Matrix;

typedef struct stop_t{
	 float r,g,b;
	 float a;
	 float pos;
}ColorStop;
typedef struct linear_g{
	char name[40];
	float x1,x2,y1,y2;
	std::list<ColorStop*> *stops; //paradas de color
}LinearGradient;
typedef struct radial_g{
	char name[40];
	float cx,cy;//center point
	//float fx,fy;
	//focus, i dont see it in synfig
	//if this value is omitted then will be = cx,cy
	float r; //radio radius
	std::list<ColorStop*> *stops; //paradas de color
}RadialGradient;

typedef struct url_t{
	int type;
	void* data;
}URL;

typedef struct vertice_t{
   	float x,y;
	float radio1,angle1;
	float radio2,angle2;
	bool split;
}Vertice;
class Svg_parser
{
		//this is inkscape oriented in some cases
public:

private:
		Gamma gamma;
	 	String filepath;
	 	String id_name;
		xmlpp::DomParser parser;
		xmlpp::Document document;
		xmlpp::Element* nodeRoot;//output
		Glib::ustring width;
		Glib::ustring height;
		Glib::ustring docname;
		int uid;
		int kux,set_canvas;
		float ox,oy;
		bool loop;//aux :D
		//urls
		std::list<LinearGradient*> lg;
		std::list<RadialGradient*> rg;

public:
		Svg_parser();
		Canvas::Handle load_svg_canvas(std::string _filepath,String &errors, String &warnings);
		//String get_id();
		//void set_id(String source);

private:        //parser bucle
		void parser_node(const xmlpp::Node* node);
		//parser headers
		void parser_svg(const xmlpp::Node* node);
		void parser_canvas(const xmlpp::Node* node);
		//layers
		void parser_layer(const xmlpp::Node* node,xmlpp::Element* root,String parent_style,Matrix* mtx_parent);
		void parser_path(const xmlpp::Node* node,xmlpp::Element* root,String parent_style,Matrix* mtx_parent);
		void parser_polygon(const xmlpp::Node* node,xmlpp::Element* root,String parent_style,Matrix* mtx_parent);
		void parser_rect(const xmlpp::Node* node,xmlpp::Element* root,String parent_style,Matrix* mtx_parent);
		//defs
		void parser_defs(const xmlpp::Node* node);
		void parser_linearGradient(const xmlpp::Node* node);
		void parser_radialGradient(const xmlpp::Node* node);
		ColorStop* newColorStop(String color,float opacity,float pos);
		LinearGradient* newLinearGradient(String name,float x1,float y1, float x2,float y2,std::list<ColorStop*> *stops);
		RadialGradient* newRadialGradient(String name,float cx,float cy,float r,std::list<ColorStop*> *stops);
		//builds urls
		void AdjustPointUrl();
		std::list<ColorStop*>* find_colorStop(String name);
		void build_url(xmlpp::Element* root, String name,Matrix *mtx);
		void build_linearGradient(xmlpp::Element* root,LinearGradient* data,Matrix* mtx);
		void build_radialGradient(xmlpp::Element* root,RadialGradient* data,Matrix* mtx);
		void build_stop_color(xmlpp::Element* root, std::list<ColorStop*> *stops);
		void build_stop_color(xmlpp::Element* root, std::list<ColorStop*> *stops,String name);
		Color adjustGamma(float r,float g,float b,float a);
		//builds
		void build_gamma(xmlpp::Element* root,float gamma);
		Matrix* build_transform(const String transform);
		void build_rotate(xmlpp::Element* root,float dx,float dy,float angle);
		void build_translate(xmlpp::Element* root,float dx,float dy);
		void build_points(xmlpp::Element* root,std::list<Vertice*> p);
		void build_vertice(xmlpp::Element* root , Vertice *p);
		void build_bline(xmlpp::Element* root,std::list<Vertice*> p,bool loop,String blinegui);
		void build_param (xmlpp::Element* root,String name,String type,String value);
		void build_param (xmlpp::Element* root,String name,String type,float value);
		void build_param (xmlpp::Element* root,String name,String type,int value);
		void build_integer (xmlpp::Element* root,String name, int value);
		void build_real (xmlpp::Element* root,String name,float value);
		void build_vector (xmlpp::Element* root,String name,float x,float y);
		void build_vector (xmlpp::Element* root,String name,float x,float y,String guid);
		void build_color(xmlpp::Element* root,float r,float g,float b,float a);
		xmlpp::Element* nodeStartBasicLayer(xmlpp::Element* root);

		//points,etc
		void coor2vect(float *x,float *y);
		void setTg2(Vertice* p,float p1x,float p1y,float p2x,float p2y);
		void setTg1(Vertice *p,float p1x,float p1y,float p2x,float p2y);
		void setSplit(Vertice* p,bool val);
		int isFirst(Vertice* nodo,float a, float b);
		Vertice* nuevoVertice(float x,float y);

		//matrix operations
		Matrix* newMatrix(float a,float b,float c,float d,float e,float f);
		Matrix* newMatrix(const String mvector);
		Matrix* newMatrix(Matrix *a);
		void transformPoint2D(Matrix *mtx,float *a,float *b);
		bool matrixVacia(Matrix* mtx);
		void composeMatrix(Matrix **mtx,Matrix *mtx1,Matrix *mtx2);
		void multiplyMatrix(Matrix **mtx1,Matrix *mtx2);
		float getRadian(float sexa);
		//attributes
		int extractSubAttribute(const String attribute, String name,String* value);
		String loadAttribute(String name,const String path_style,const String master_style,const String subattribute,const String defaultVal);
		String loadAttribute(String name,const String path_style,const String master_style,const String defaultVal);
		std::vector<String> get_tokens_path(String path);
		std::list<std::list<Vertice*> > parser_path_d(String path_d,Matrix* mtx);
		int randomLetter();
		int getRed(String hex);
		int getGreen(String hex);
		int getBlue(String hex);
		int hextodec(String hex);
		float getDimension(const String ac);
		//funciones string
		void removeS(String *input);
		void removeIntoS(String *input);
		std::vector<String> tokenize(const String& str,const String& delimiters);
		void get_canvas_name(String _filepath);
		String new_guid();
};
// END of Svg_parser class

/* === E X T E R N S ======================================================= */

extern Canvas::Handle open_svg(std::string _filepath,String &errors, String &warnings);

}; // END of namespace synfig

/* === E N D =============================================================== */

#endif