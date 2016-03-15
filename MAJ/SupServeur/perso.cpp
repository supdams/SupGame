// perso.cpp

#include "perso.hpp"
#include "database.hpp"

database BDD;
#define HOST "localhost"
#define USER "root"
#define PASS "123456"
#define DATABASE "comptes"


perso::perso(string nom, int posx, int posy, string id, string race)
{
	this->nom_=nom;
	this->posx_=posx;
	this->posy_=posy;
	this->id_=id;
	this->race_=race;
}

void perso::update_position(int posx, int posy)
{
	this->posx_=posx;
	this->posy_=posy;
}

int connect(char *host, char *table, char *user, char *pass)
{
	int errormsg = 0; //catch any database errors
	// dbConnection = true; //connection started, toggle flag

	//connect to the database
	errormsg = BDD.openConnection(host, table, user, pass);

	cout<<"sql: "<<errormsg<< endl;    
	//return the database message
	return errormsg;
}


string perso::new_liste_creature(string map,JeuComplet *jeucomplet)
{
	cout<<"new_liste_creature"<< endl;
	
	string data="";
	
	for(int i = 0; i < jeucomplet->jeu.nbMonstre ; i ++)
	{
		data  = 
			data +  outils::charToString(_SEPARATEUR_MESSAGE_) +
			outils::itos(jeucomplet->jeu.monstre[i].id) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
			outils::itos(jeucomplet->jeu.monstre[i].map) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
			outils::itos(jeucomplet->jeu.monstre[i].modelid) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
			jeucomplet->jeu.monstre[i].nom_monstre + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
			outils::itos(jeucomplet->jeu.monstre[i].position_x) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + //test ou -1 ;;;
			outils::itos(jeucomplet->jeu.monstre[i].position_y) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
			outils::itos(jeucomplet->jeu.monstre[i].spawntimesecs) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
			outils::itos(jeucomplet->jeu.monstre[i].hp) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
			outils::itos(jeucomplet->jeu.monstre[i].mindmg) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
			outils::itos(jeucomplet->jeu.monstre[i].maxdmg) + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
			outils::itos(jeucomplet->jeu.monstre[i].DeathState);		

	}

	this->acces_ = outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(jeucomplet->jeu.nbMonstre) + data + outils::charToString(_SEPARATEUR_MESSAGE_) + outils::charToString(_FIN_MESSAGE_);
	
	//cout<<"this->acces_this->acces_this->acces_this->acces_this->acces_ "<<this->acces_<< endl;
	
	return this->acces_;
}

string perso::liste_creature(string map)
{

	cout<<"liste_creature"<< endl;
	 map ="1"; // temporaire recup la map du joueur...
	if (connect(HOST, DATABASE, USER, PASS) == SUCCESS)
	{
		cout<<"connect"<< endl; 
		char query[500];
		sprintf(query, "SELECT * FROM creature WHERE map='%d'", stoi(map));
		cout<<"SQL :"<<query<< endl; 


		if (BDD.query(query))
		{
			cout<<"BDD.NumRows() :"<<BDD.NumRows()<< endl;
			if (BDD.NumRows()==0){
				cout<<"result ERREURRRRRRRRR"<< endl;
			}
			else
			{
				cout<<"Il y a "<<BDD.NumRows()<<" creature"<< endl;
				
				MYSQL_ROW row=NULL;
				int i=0;
				
				string tmp_data=outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + map ;


				this->acces_ = outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(i);
				this->acces_ = this->acces_ + tmp_data +outils::charToString(_SEPARATEUR_MESSAGE_PERSO_);
				
				while ((row = BDD.GetRow()) != NULL)
				{
					i++;

					cout
					<<"creature num :"<<i
					<<" id :"<<row[0]
					<<" map :"<<row[1]
					<<" modelid :"<<row[2]
					<<" nom :"<<row[3]
					<<" position_x :"<<row[4]
					<<" position_y :"<<row[5]
					<<" spawntimesecs :"<<row[6]
					<<" hp :"<<row[7]
					<<" mindmg :"<<row[8]
					<<" maxdmg :"<<row[9]
					<<" DeathState :"<<row[10]
					<< endl;

					tmp_data  = tmp_data +  outils::charToString(_SEPARATEUR_MESSAGE_) + 
						row[0] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[1] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[2] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[3] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[4] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[5] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[6] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[7] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[8] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[9] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[10];		
				} 
						this->acces_ = outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(i);
						this->acces_ = this->acces_ + tmp_data +outils::charToString(_SEPARATEUR_MESSAGE_);
						this->acces_ += outils::charToString(_FIN_MESSAGE_);

			}

		}
	}

	return this->acces_;
}


string perso::creation_perso(string pseudo, string guide)
{
	cout<<"creation_perso"<< endl; 
	cout<<"pseudo: "<<pseudo<<endl;
	cout<<"acc: "<<guide<<endl;// on utilise guilde pr pas refaire un pr rien lol
	if (connect(HOST, DATABASE, USER, PASS) == SUCCESS)
	{
		cout<<"connect"<< endl; 
		char query[500];
		sprintf(query, "SELECT * FROM characters WHERE name='%s'", pseudo.c_str());
		cout<<"SELECT * FROM characters WHERE name="<<pseudo<< endl; 
		string tmp_data;

		if (BDD.query(query))
		{
			cout<<"BDD.NumRows() :"<<BDD.NumRows()<< endl;
			if (BDD.NumRows()==0){
				cout<<"ok il na pas de nom pareil"<< endl;

		char query2[500];
		
		sprintf(query2, "INSERT INTO characters (guid, account, name, race, classe, position_x, position_y, map, data) VALUES (NULL, '%d', '%s', '1', '1', '100', '100', '1', 'rien')",stoi(guide.c_str()),pseudo.c_str());

		cout<<"TESTTT QUERY 2"<< endl;

if (BDD.query(query2))
		{
			cout<<"TESTTT QUERY 2 OKKKKKK"<< endl;
			}
				tmp_data="OK";
			}
			else
			{
				cout<<"ERREUR il y a deja ce nom ^^ "<< endl;
				tmp_data="ERREUR";
			}


				this->acces_ = outils::charToString(_SEPARATEUR_MESSAGE_PERSO_);
				this->acces_ = this->acces_ + tmp_data;
				this->acces_ += outils::charToString(_FIN_MESSAGE_);

		}
	}

	return this->acces_;
}




string perso::connect_perso(string guid)
{

	cout<<"connect_perso"<< endl; 
	cout<<"guid: "<<guid<<endl;
	if (connect(HOST, DATABASE, USER, PASS) == SUCCESS)
	{
		cout<<"connect"<< endl; 
		char query[500];
		sprintf(query, "SELECT * FROM characters WHERE guid='%d'", stoi(guid));
		cout<<"SELECT * FROM characters WHERE guid="<< endl; 


		if (BDD.query(query))
		{
			cout<<"BDD.NumRows() :"<<BDD.NumRows()<< endl;
			if (BDD.NumRows()==0){
				cout<<"result ERREURRRRRRRRR"<< endl;
			}
			else
			{
				cout<<"LE compte a "<<BDD.NumRows()<<" perso"<< endl;
				MYSQL_ROW row=NULL;
				int i=0;
				string tmp_data="";


				while ((row = BDD.GetRow()) != NULL)
				{
					i++;

					cout<<"Perso num :"<<i
						<<" GUID :"<<row[0]
					<<" ACC :"<<row[1]
					<<" NAME :"<<row[2]
					<<" race :"<<row[3]
					<<" classe :"<<row[4]
					<<" position_x :"<<row[5]
					<<" position_y :"<<row[6]
					<<" map :"<<row[7]
					<<" data :"<<row[8]<< endl;

					tmp_data  = 
						row[2] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[0] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[3] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[5] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[6] + outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + 
						row[7];		
				} 


				this->acces_ = outils::charToString(_SEPARATEUR_MESSAGE_PERSO_);
				this->acces_ = this->acces_ + tmp_data;
				this->acces_ += outils::charToString(_FIN_MESSAGE_);
			}

		}
	}

	return this->acces_;
}



string perso::recharger_liste_perso(string guid)
{

	cout<<"recharger_liste_perso"<< endl; 
	cout<<"account: "<<guid<<endl;
	if (connect(HOST, DATABASE, USER, PASS) == SUCCESS)
	{
		cout<<"connect"<< endl; 
		char query[500];
		sprintf(query, "SELECT * FROM characters WHERE account='%d'", stoi(guid));
		cout<<"SELECT * FROM characters WHERE guid="<< endl; 


		if (BDD.query(query))
		{
			cout<<"BDD.NumRows() :"<<BDD.NumRows()<< endl;
			if (BDD.NumRows()==0){
				cout<<"result ERREURRRRRRRRR"<< endl;
			}
			else
			{
				cout<<"LE compte a "<<BDD.NumRows()<<" perso"<< endl;
				MYSQL_ROW row=NULL;
				int i=0;


					string tmp_data=outils::charToString(_SEPARATEUR_MESSAGE_) + guid ;


						this->acces_ = outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(i);
						this->acces_ = this->acces_ + tmp_data +outils::charToString(_SEPARATEUR_MESSAGE_);

				while ((row = BDD.GetRow()) != NULL)
				{
					i++;

					cout<<"Perso num :"<<i
						<<" GUID :"<<row[0]
					<<" ACC :"<<row[1]
					<<" NAME :"<<row[2]
					<<" race :"<<row[3]
					<<" classe :"<<row[4]
					<<" position_x :"<<row[5]
					<<" position_y :"<<row[6]
					<<" map :"<<row[7]
					<<" data :"<<row[8]<< endl;

					tmp_data  = tmp_data +  
								outils::charToString(_SEPARATEUR_MESSAGE_) + row[2] + 
								outils::charToString(_SEPARATEUR_MESSAGE_) + row[0] +
								outils::charToString(_SEPARATEUR_MESSAGE_) + row[3];	
				} 
				

						this->acces_ = outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(i);
						this->acces_ = this->acces_ + tmp_data +outils::charToString(_SEPARATEUR_MESSAGE_);
this->acces_ += outils::charToString(_FIN_MESSAGE_);
			}

		}
	}

	return this->acces_;
}



string perso::load_login(string pseudo, string mdp)
{
	cout<<"load_login"<< endl; 
	if (connect(HOST, DATABASE, USER, PASS) == SUCCESS)
	{
		cout<<"connect"<< endl; 
		char query[500];
		sprintf(query, "SELECT id FROM comptes WHERE pseudo='%s' AND mdp='%s'", pseudo.c_str(), mdp.c_str());
		cout<<"SELECT id FROM comptes WHERE pseudo="<< endl; 


		if (BDD.query(query))
		{
			cout<<"BDD.NumRows() :"<<BDD.NumRows()<< endl;
			if (BDD.NumRows()==0){
				cout<<"result ERREURRRRRRRRR"<< endl;
				this->acces_="load_login_refuser";
			}
			else
			{
				id_compte=BDD.intQuery(query);// pr donner l id des perso
				cout<<"id_compte :"<<id_compte<< endl;

				string tmp_data=outils::charToString(_SEPARATEUR_MESSAGE_) + outils::itos(id_compte) ;

				
				char query2[500];
				sprintf(query2, "SELECT * FROM characters WHERE account='%d'", id_compte);

				if (BDD.query(query2))
				{
					int i=0;
					if (BDD.NumRows()==0){
						cout<<"result query2 ERREURRRRRRRRR"<< endl;
						this->acces_ = outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(i);
						this->acces_ = this->acces_ + tmp_data +outils::charToString(_SEPARATEUR_MESSAGE_);
						this->acces_="load_login_reussi" + this->acces_;
					}else{
						cout<<"LE compte a "<<BDD.NumRows()<<" perso"<< endl;
						MYSQL_ROW row=NULL;
						
						


						while ((row = BDD.GetRow()) != NULL)
						{
							i++;

							cout<<"Perso num :"<<i
								<<" GUID :"<<row[0]
							<<" ACC :"<<row[1]
							<<" NAME :"<<row[2]
							<<" race :"<<row[3]
							<<" classe :"<<row[4]
							<<" position_x :"<<row[5]
							<<" position_y :"<<row[6]
							<<" map :"<<row[7]
							<<" data :"<<row[8]<< endl;

							tmp_data  =tmp_data + 
								outils::charToString(_SEPARATEUR_MESSAGE_) + row[2] + 
								outils::charToString(_SEPARATEUR_MESSAGE_) + row[0] +
								outils::charToString(_SEPARATEUR_MESSAGE_) + row[3];		
						} 


						this->acces_ = outils::charToString(_SEPARATEUR_MESSAGE_PERSO_) + outils::itos(i);
						this->acces_ = this->acces_ + tmp_data +outils::charToString(_SEPARATEUR_MESSAGE_);
						this->acces_="load_login_reussi" + this->acces_;

					}
				}

				
				this->acces_ += outils::charToString(_FIN_MESSAGE_);


				cout<<"result: OKKKKKKKKKKK"<< endl; 
				//cout<<"BDD: "<<BDD.intQuery(query)<< endl;


			}
		}
	}

	return this->acces_;
}

string perso::getPos()
{
	string ret="";
	char pos[_MAXLEN_];

	sprintf(pos,"%-3d%-3d", this->posx_,this->posy_);
	ret = outils::ctos(pos);	
	return ret;

}

int perso::getX(){	return (this->posx_); }
int perso::getY(){	return (this->posy_); }

string perso::getNom(){ return (this->nom_); }
string perso::getId(){ return (this->id_); }
string perso::getRace(){ return (this->race_); }