#ifndef _SOUND_H_
#define _SOUND_H_ 1


/************************************************************************
La classe client permet de gérer l'interface réseaux du 'jeu'.
Elle contient les méthodes pour se connecter au serveur et échanger 
des messages avec lui.
**************************************************************************/
class sound{
public:
sound();
~sound();
void playWAV(const char *wav);
void playMP3(const char *mp3);
};

#endif