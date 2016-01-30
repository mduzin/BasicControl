/*
 * xml_parser.h
 *
 *  Created on: Aug 27, 2015
 *      Author: mduzinki
 */

#ifndef XML_PARSER_H_
#define XML_PARSER_H_

/**
 * Zbior funckji bibliotecznych potrzebych do obrobki pliku .xml z
 * parametrami testow do przeprowadzenia
 *
 */

#include "common.h"
#include "file_lib.h"
#include "sim_env.h"
#include "regulator_PID.h"
#include "model_1.h"
#include "test_env.h"

/*
 * @brief Funkcja wyszukuje w pliku okreslony znacznik(tag)
 *
 * */
STATUS XmlGetValue(IN char* tagname, OUT double *value)
{



}



#endif /* XML_PARSER_H_ */
