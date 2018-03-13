/*
 * LabProcessor.cpp
 *
 *  Created on: 18 juil. 2012
 *      Author: golgauth
 */

#include "LabProcessor.h"

namespace elp {

LabProcessor::LabProcessor() {
	// TODO Auto-generated constructor stub

}

LabProcessor::~LabProcessor() {
	// TODO Auto-generated destructor stub
}


void LabProcessor::ProcessPlugin(string plug_name)
{
	/*
	 *  TODO :
	 *
	 * - >>>>>>>  First of all : Bien potasser la doc de Blitz  !!!!!!!!!!!!!!!!
	 *
	 * - Pour les histoires de voisinnage, voir ce que propose Blitz pour chopper des sous ensembles de tableaux dans
	 * 		un tableau avec les Ranges (page 26)
	 *
	 * - Trouver un moyen de renseigner les constantes obligatoires (lister des #defines ??? utliser la VarFactory ???)
	 * - Créer la loop globale timée
	 * - Implementer une lib supplementaire accessible par les plugins avec des fonctions specifiques
	 * 		(ex. Creation de reseaux de sites, ajout d'un etat, ajout d'un event, ...)
	 * 		Cette lib contiendra egalement des tags ou STENCILS (voir doc Blitz++) pour ouverture / fermeture d'un event, ...
	 * - Implementer les objets necessaires Events, Hosts, ... => listes dans le processor ???
	 * 		(Note : un host pourra par ex. avoir un nombre dynamique d'etats. machine a etat infinie ???)
	 * - Dans le processor (ou dans le 'plugin.cpp.template', a voir...) : toujours 3 bloc :
	 * 			+ Initialisation des rates et variables globales (Cf. parvir) + Init des propensy stuffs + etats possibles
	 * 			+ Choix d'un event (switch sur les events => Stack d'events a faire gerer par l'EventManager ?)
	 * 				(transitions pour les hosts a gerer comme une machine a etats : voir etats possibles...)
	 * 			+ Mise-a-jour des propensy stuffs
	 * - Implementer parvir, puis les fichiers de seb avec ce system pour bien voir ce qui est necessaire en terme de
	 * 		fonctionnalités, flexibilité, généricité, généralisation du model objet pour des taches plus diverses...
	 * - Pour les commandes perso additionnelles, prevoir un pattern Command/Execute...
	 * - Faire une librairie bien balaise en surcouche de Blitz that covers all our needs and more
	 *
	 */

	const double TMAX = 1000000.0;
	double t = 0.0;
	do
	{
		LabPluginsManager *pluginsManager = LabPluginsManager::GetInstance();
		pluginsManager->ExecutePlugin(plug_name, false);
		t += 1.0;
	} while (t < TMAX);
}

} /* namespace elp */
