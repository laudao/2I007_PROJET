/* NGUYEN Kim-Anh Laura et HAMISSI Fatemeh 
	 A compiler avec:
	 - gcc projet.c -I /usr/X11R6/include -L /usr/X11R6/lib -lX11 -o projet (Mac OS X)
	 - gcc -o projet projet.c -lX11 (Linux)
 */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define min(x, y)	((x) < (y) ? (x) : (y))
#define max(x, y)	((x) > (y) ? (x) : (y))

enum forme {CERCLE , RECTANGLE, LIGNE, RECTANGLEPLEIN, CERCLEPLEIN};
typedef enum forme forme; 

Display*       dpy;
Window         wracine, wprincipale, wmenu, wchat, wtextinput;
Window         wcercle, wrectangle, wrectangleplein, wcercleplein, wligne;
Window         wrouge, wbleu, wvert, wnoir, worange, wjaune, wviolet;
Window         wepaisseur1, wepaisseur2,wepaisseur3,wepaisseur4, wepaisseur;
int            ecran;
XEvent         evmt;
GC             ctx,ctx_xor;
unsigned long  cnoir, cblanc, cvert, cjaune, corange, cviolet, crouge, cbleu, cgris, ma_couleur, couleur_dessinateur;
Atom           XA_FORME, XA_COULEUR, XA_EPAISSEUR, XA_xA, XA_xB,XA_yA,XA_yB, XA_CHAT; 
int verifForme, verifCouleur, verifEpaisseur, verifxA, verifxB, verifyA, verifyB;
char chaine_retour[200];
unsigned char *donnees_retournees;
char           chaine[200]; /* le texte saisi par l'utilisateur */
Atom           type_effectif_retour;
int            format_effectif_retour;
int nb_lus = 0;
unsigned long  nb_lus_retour;
unsigned long nb_octets_restants_retour;
int            xA,xB, yA,yB;
int            bouton; 
int			   nb_boutons_enfonces;
forme          ma_forme, forme_dessinateur;
int            touche; 
int            mon_epaisseur, epaisseur_dessinateur; 
// Le curseur pour le texte dans le wchat
int            cury = 0;
int maj_caps_lock = 0;
int maj_shift = 0;

void Installer(char *serveur);
void PourButtonPress(XButtonPressedEvent *evmt);
void PourButtonRelease(XButtonReleasedEvent *evmt);
void PourMotionNotify(XPointerMovedEvent *evmt);
void PourKeyPress (XKeyPressedEvent *evmt);
void PourKeyRelease (XKeyReleasedEvent *evmt);
void PourExpose(XExposeEvent *evmt);
void PourPropertyNotify(XPropertyEvent *evmt);
void Draw(forme forme_dessin, int couleur_dessin, int epaisseur_dessin, GC ctx, int xA, int xB, int yA, int yB);

int main (int argc, char *argv[]){  /* la procedue main()                */
		Installer(NULL);
		for (;;){       /* la boucle d'evenements            */
				XNextEvent(dpy, &evmt);
				switch (evmt.type){
						case ButtonPress :
								PourButtonPress((XButtonPressedEvent *)&evmt);
								break;
						case ButtonRelease :
								PourButtonRelease((XButtonReleasedEvent *)&evmt);
								break;
						case MotionNotify :
								PourMotionNotify((XPointerMovedEvent *)&evmt);
								break;
						case Expose:
								PourExpose((XExposeEvent *)&evmt);
								break;
						case KeyPress:
								PourKeyPress((XKeyPressedEvent*)&evmt);
								break;
						case PropertyNotify:
							if (evmt.xproperty.state == PropertyNewValue){
								if (evmt.xproperty.atom == XA_FORME){
										XGetWindowProperty(dpy, wprincipale, XA_FORME, 0, 100, False, XA_INTEGER, &type_effectif_retour, &format_effectif_retour, &nb_lus_retour, &nb_octets_restants_retour, &donnees_retournees);
										printf("%s\n", donnees_retournees);
										strncpy(chaine_retour, donnees_retournees, nb_lus_retour);
										printf("%s\n", chaine_retour);
								}
							}
						
						default:;
				}
		}
}

void Installer (char *serveur){
		XSetWindowAttributes attr;

		dpy = XOpenDisplay(serveur);
		if (dpy == NULL){ 
				printf("Connection impossible avec le serveur %s.\n", serveur);
				exit(0);
		}

		wracine   = DefaultRootWindow(dpy);
		ecran     = DefaultScreen(dpy);

		cnoir = 0x000000;
		cblanc = 0xFFFFFF;
		crouge = 0xFF0000;
		cvert = 0x00FF00;
		cbleu = 0x0000FF;
		corange = 0xFF5500;
		cjaune = 0xFFFF00;
		cviolet = 0x9700FC;
		cgris = 0xEFEFEF;

		ma_couleur = cnoir; 
		ma_forme = CERCLEPLEIN;
		mon_epaisseur = 1;
		nb_boutons_enfonces = 0;
		cury = 15;

		wprincipale = XCreateSimpleWindow(dpy, wracine, 0, 0, 500, 500, 0,  cnoir, cblanc);
	
		attr.override_redirect= True; 
		wmenu = XCreateWindow(dpy, wracine, 0, 0, 150, 250, 1,
						CopyFromParent, CopyFromParent, CopyFromParent,
						CWOverrideRedirect, &attr);
		wrouge = XCreateSimpleWindow(dpy, wmenu, 0, 0, 50, 25, 0, cnoir, crouge);
		wvert  = XCreateSimpleWindow(dpy, wmenu, 50, 0, 50, 25, 0, cnoir, cvert);
		wbleu  = XCreateSimpleWindow(dpy, wmenu, 100, 0, 50, 25, 0, cnoir, cbleu);		
		worange = XCreateSimpleWindow(dpy, wmenu, 0, 25, 50, 25, 0, cnoir, corange);
		wjaune  = XCreateSimpleWindow(dpy, wmenu, 50, 25, 50, 25, 0, cnoir, cjaune);
		wviolet = XCreateSimpleWindow(dpy, wmenu, 100, 25, 50, 25, 0, cnoir, cviolet);	
		wnoir = XCreateSimpleWindow(dpy, wmenu, 0, 50, 150, 25, 0, cnoir, cnoir);
		wcercle = XCreateSimpleWindow(dpy, wmenu, 0,75 , 150, 25, 0, cnoir, cblanc);
		wrectangle = XCreateSimpleWindow(dpy, wmenu, 0, 100, 150, 25, 0, cnoir, cblanc);
		wrectangleplein = XCreateSimpleWindow(dpy, wmenu,0, 125, 150, 25, 0, cnoir, cblanc);
		wcercleplein = XCreateSimpleWindow(dpy, wmenu,0, 150, 150, 25, 0, cnoir, cblanc);
		wligne = XCreateSimpleWindow(dpy, wmenu,0,175, 150, 25, 0, cnoir, cblanc);
		wepaisseur = XCreateSimpleWindow(dpy, wmenu,0,200, 150, 25, 0, cnoir, cblanc);
		wepaisseur1 = XCreateSimpleWindow(dpy, wmenu,0,225, 25, 25, 0, cnoir, cblanc);
		wepaisseur2 = XCreateSimpleWindow(dpy, wmenu,25,225, 25, 25, 0, cnoir, cblanc);
		wepaisseur3 = XCreateSimpleWindow(dpy, wmenu,50,225, 25, 25, 0, cnoir, cblanc);
		wepaisseur4 = XCreateSimpleWindow(dpy, wmenu,75,225, 25, 25, 0, cnoir, cblanc);

		XSelectInput(dpy, wrouge, ButtonReleaseMask);
		XSelectInput(dpy, wvert, ButtonReleaseMask);
		XSelectInput(dpy, wbleu, ButtonReleaseMask);
		XSelectInput(dpy, worange, ButtonReleaseMask);
		XSelectInput(dpy, wjaune, ButtonReleaseMask);
		XSelectInput(dpy, wviolet, ButtonReleaseMask);
		XSelectInput(dpy, wnoir, ButtonReleaseMask);
		XSelectInput(dpy, wepaisseur, ExposureMask);

		XSelectInput(dpy, wrectangle, ButtonReleaseMask|ExposureMask);
		XSelectInput(dpy, wrectangleplein, ButtonReleaseMask|ExposureMask);
		XSelectInput(dpy, wcercle, ButtonReleaseMask|ExposureMask);
		XSelectInput(dpy, wcercleplein, ButtonReleaseMask|ExposureMask);
		XSelectInput(dpy, wligne, ButtonReleaseMask|ExposureMask);
		XSelectInput(dpy, wepaisseur1, ButtonReleaseMask|ExposureMask);
		XSelectInput(dpy, wepaisseur2, ButtonReleaseMask|ExposureMask);
		XSelectInput(dpy, wepaisseur3, ButtonReleaseMask|ExposureMask);
		XSelectInput(dpy, wepaisseur4, ButtonReleaseMask|ExposureMask);

		wchat = XCreateSimpleWindow(dpy, wracine, 0, 0, 200, 300, 0, cnoir, cblanc);
		wtextinput = XCreateSimpleWindow(dpy,wchat,0,270,200,30,0, cnoir, cgris); 
		XSelectInput(dpy, wtextinput, KeyPressMask|PropertyChangeMask);
		XSelectInput(dpy, wchat, KeyPressMask);
		ctx = XCreateGC(dpy, wprincipale, 0, NULL);
		XSetLineAttributes(dpy, ctx, mon_epaisseur, LineSolid, CapButt, JoinMiter);
		XSetForeground(dpy, ctx, cnoir);

		ctx_xor = XCreateGC(dpy, wprincipale, 0, NULL);
		XSetForeground(dpy, ctx_xor, cblanc);
		XSetFunction(dpy, ctx_xor, GXxor);



		XSelectInput(dpy, wprincipale,
						ButtonPressMask|ButtonReleaseMask|ButtonMotionMask|
						OwnerGrabButtonMask|PropertyChangeMask);

		XStoreName(dpy,wprincipale, "Dessin");
		XStoreName(dpy,wchat, "Chat");
		XMapWindow(dpy,wprincipale);
		XMapWindow(dpy,wchat);
		XMapSubwindows(dpy,wchat); 
		XMapSubwindows(dpy,wmenu);

		XA_CHAT = XInternAtom(dpy, "CHAT", False);

		/* ????????????????????????? */

		XA_FORME = XInternAtom(dpy, "FORME", False);
		XA_COULEUR = XInternAtom(dpy, "COULEUR", False);
		XA_EPAISSEUR = XInternAtom(dpy, "EPAISSEUR", False);
		XA_xA = XInternAtom(dpy, "xA", False);
		XA_xB = XInternAtom(dpy, "xB", False);
		XA_yA = XInternAtom(dpy, "yA", False);
		XA_yB = XInternAtom(dpy, "yB", False);

		XSetSelectionOwner(dpy, XA_FORME, wprincipale, CurrentTime);
		
		XSetIOErrorHandler(XCloseDisplay);
}

void Draw(forme forme_dessin, int couleur_dessin, int epaisseur_dessin, GC ctx, int xA, int xB, int yA, int yB){

		switch(forme_dessin){
				case LIGNE : 
						XDrawLine(dpy, wprincipale, ctx, xA, yA, xB, yB);
						break;
				case CERCLE :  
						XDrawArc(dpy, wprincipale, ctx , xA, yA, abs(xA-xB), abs(yA-yB), 0,360 *64);
						break;
				case CERCLEPLEIN :    
						XFillArc(dpy, wprincipale, ctx , xA, yA, abs(xA-xB), abs(yA-yB), 0,360 *64);
						break;
				case RECTANGLE :
						XDrawRectangle(dpy, wprincipale, ctx,xA ,yA ,abs(xA-xB),abs(yA-yB));
						break;
				case RECTANGLEPLEIN :
						XFillRectangle(dpy, wprincipale, ctx,xA ,yA ,abs(xA-xB),abs(yA-yB));
						break;
				default : ; 
		}
		/* ????????????????????????????? */
}

void PourButtonPress(XButtonPressedEvent *evmt){
		if (!nb_boutons_enfonces) {
				bouton = evmt->button;
				switch (bouton) {
						case 3 :
								XMoveWindow(dpy, wmenu, evmt->x_root, evmt->y_root);
								XMapRaised(dpy, wmenu);
								break;
						case 1 :
								xA = xB = evmt->x;
								yA = yB = evmt->y;

								/* le premier trait avec xor  */
								Draw(ma_forme,ma_couleur,mon_epaisseur,ctx_xor,xA, xB, yA,yB);
								break;
						default :;
				}
		}
		nb_boutons_enfonces = 1;
}

void PourButtonRelease (XButtonReleasedEvent *evmt) {
		nb_boutons_enfonces = 0;
		if (evmt->button == bouton) {
				switch (bouton) {
						case 3 :
								XUnmapWindow(dpy, wmenu);
								if (evmt -> window == wrouge){
										ma_couleur=crouge;
								}
								if (evmt -> window == worange){
										ma_couleur=corange;
								}
								if (evmt -> window == wvert){
										ma_couleur=cvert;
								}
								if (evmt -> window == wbleu){
										ma_couleur=cbleu;
								}
								if (evmt -> window == wjaune){
										ma_couleur=cjaune;
								}
								if (evmt -> window == wviolet){
										ma_couleur=cviolet;
								}
								if (evmt -> window == wnoir){
										ma_couleur=cnoir;
								}
								if (evmt -> window == wrectangle){
										ma_forme = RECTANGLE;
								}
								if (evmt -> window == wrectangleplein){
										ma_forme = RECTANGLEPLEIN;
								}
								if (evmt -> window == wcercle){
										ma_forme = CERCLE;
								}
								if (evmt -> window == wcercleplein){
										ma_forme = CERCLEPLEIN;
								}
								if (evmt -> window == wligne){
										ma_forme = LIGNE;
								}
								if (evmt -> window == wepaisseur1){
										mon_epaisseur = 1;
								}
								if (evmt -> window == wepaisseur2){
										mon_epaisseur = 2;
								}		
								if (evmt -> window == wepaisseur3){
										mon_epaisseur = 3;
								}		
								if (evmt -> window == wepaisseur4){
										mon_epaisseur = 4;
								}
								XSetForeground(dpy, ctx_xor,cblanc-ma_couleur );
								XSetForeground(dpy, ctx , ma_couleur);
								XSetLineAttributes(dpy, ctx, mon_epaisseur, LineSolid, CapButt, JoinMiter);
								XClearWindow(dpy, wmenu);

								break;

						case 1 :
								/* display, fenetre, nom de la propriete, type de la propriete, 32 bits, substituer, donnee et nombre d'elements */
								XChangeProperty(dpy, wprincipale, XA_FORME, XA_STRING, 32, PropModeReplace, (unsigned char*) &ma_forme, 1);
								XChangeProperty(dpy, wprincipale, XA_COULEUR, XA_INTEGER, 32, PropModeReplace, (unsigned char *) &XA_COULEUR, 1);
								XChangeProperty(dpy, wprincipale, XA_EPAISSEUR, XA_INTEGER, 32, PropModeReplace, (unsigned char*) &XA_EPAISSEUR, 1);
								XChangeProperty(dpy, wprincipale, XA_xA, XA_INTEGER, 32, PropModeReplace, (unsigned char *) &XA_xA, 1);
								XChangeProperty(dpy, wprincipale, XA_xB, XA_INTEGER, 32, PropModeReplace, (unsigned char *) &XA_xB, 1);
								XChangeProperty(dpy, wprincipale, XA_yA, XA_INTEGER, 32, PropModeReplace, (unsigned char *) &XA_yA, 1);
								XChangeProperty(dpy, wprincipale, XA_yB, XA_INTEGER, 32, PropModeReplace, (unsigned char *) &XA_yB, 1);
								Draw(ma_forme,ma_couleur,mon_epaisseur,ctx_xor,xA, xB, yA,yB);
								Draw(ma_forme,ma_couleur,mon_epaisseur,ctx,xA, xB, yA,yB);
								break;
						default :;
				}
				bouton = 0;
		}
}

void PourMotionNotify (XPointerMovedEvent *evmt) {
		if (bouton == 1) {
				/* effacer */
				Draw(ma_forme,ma_couleur,mon_epaisseur,ctx_xor ,xA, xB, yA,yB);
				xB = evmt->x;
				yB = evmt->y;
				/* dessiner un nouveau */
				Draw(ma_forme,ma_couleur,mon_epaisseur,ctx_xor,xA, xB, yA,yB);
		}
}


void PourKeyPress (XKeyPressedEvent *evmt) {
	char *keycode; /* lettre correspondant a la touche appuyee */
	int i;	
	touche = XKeycodeToKeysym(dpy, evmt -> keycode, 0); /* keysym de la touche appuyee */
	
	/* envoi du message */
	if (touche == 0xff0d){
		cury = cury + 10; /* incrementation du curseur dans le chat */
		XClearWindow(dpy, wtextinput); /* on clear la fenetre de saisie */
		
		if (cury >= 260) { /* si le chat est rempli */
				XClearWindow(dpy, wchat); /* on nettoie le chat */
				cury=0; /* on remet le curseur a 0 */
		}

		/* on ecrit la chaine de caracteres dans le chat */
		XDrawString(dpy,wchat, DefaultGC(dpy, ecran),5, cury, chaine, nb_lus);
		nb_lus = 0; 

		/* on reinitialise la chaine */
		for (i = 0; i < 200; i++){
			chaine[i] = 0;
		}
	}
	else if (nb_lus == 30){
		
	}
	/* supprimer */
	else if (touche == 0xff08){
		chaine[nb_lus] = '\0'; 
		(nb_lus)--; /* un caractere en moins */
		XClearWindow(dpy, wtextinput);
		XDrawString(dpy,wtextinput, DefaultGC(dpy, ecran), 10, 10,chaine,nb_lus);
	}
	/* espace */
	else if (touche == 0x0020){
		chaine[nb_lus] = ' ';
		nb_lus++; /* on ajoute l'espace */
		XClearWindow(dpy, wtextinput);
		XDrawString(dpy,wtextinput, DefaultGC(dpy, ecran), 10, 10,chaine,nb_lus);
		
	}
	/* majuscule caps lock*/
	else if (touche == 0xffe5){
		if (maj_caps_lock){
			maj_caps_lock = 0;
		}else{
			maj_caps_lock = 1;
		}
	}
	/* majuscule shift */
	else if ((touche == 0xffe1) || (touche == 0xffe2)){
		maj_shift = 1;
	}
	else{
		keycode = XKeysymToString(XKeycodeToKeysym(dpy, evmt -> keycode, 0));

		chaine[nb_lus] = keycode[0]; /* ajout de la lettre dans la chaine */
		
		if (maj_caps_lock == 1 || maj_shift == 1){
			if ((touche < 0x0030) || (touche > 0x0039)){
				chaine[nb_lus] = chaine[nb_lus] - 32;
			}
			else{
				printf("coucou\n");
				chaine[nb_lus] = touche % 16;
			}
			if (maj_shift){
				maj_shift = 0;
			}
		}
		
		nb_lus++;
		XClearWindow(dpy, wtextinput);
		XDrawString(dpy,wtextinput, DefaultGC(dpy, ecran), 10, 10,chaine,nb_lus);
		
		keycode = NULL;
	}
}

void PourExpose(XExposeEvent *evmt){
		XDrawString(dpy,wrectangle , DefaultGC(dpy, ecran),15,12,"rectangle vide",14);
		XDrawString(dpy,wrectangleplein , DefaultGC(dpy, ecran),15,12,"rectangle plein",15);
		XDrawString(dpy, wcercle, DefaultGC(dpy, ecran),15,12,"ellipse vide",12);
		XDrawString(dpy, wcercleplein, DefaultGC(dpy, ecran), 15,12,"ellipse pleine",14);
		XDrawString(dpy, wligne, DefaultGC(dpy, ecran),15,12,"ligne",5);
		XDrawString(dpy, wepaisseur , DefaultGC(dpy, ecran),15,12,"epaisseur du trait :",20);
		XDrawString(dpy, wepaisseur1, DefaultGC(dpy, ecran),15,12,"1",1);
		XDrawString(dpy, wepaisseur2, DefaultGC(dpy, ecran), 15,12,"2",1);
		XDrawString(dpy, wepaisseur3, DefaultGC(dpy, ecran),15,12,"3",1);
		XDrawString(dpy, wepaisseur4, DefaultGC(dpy, ecran),15,12,"4",1);

}
