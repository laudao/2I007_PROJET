
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#include <string.h>
#include <unistd.h>

#define min(x, y)	((x) < (y) ? (x) : (y))
#define max(x, y)	((x) > (y) ? (x) : (y))

enum forme {CERCLE , RECTANGLE, LIGNE, RECTANGLEPLEIN, CERCLEPLEIN};
typedef enum forme forme; 

Display*       dpy;
Window         wracine, wprincipale, wmenu, wchat, wtextinput;
Window         wcercle, wrectangle, wrectangleplein, wcercleplein, wligne;
Window         wrouge, wbleu, wvert, wnoir, worange, wjaune, wviolet;
Window         wepaisseur1, wepaisseur2,wepaisseur3,wepaisseur4;
int            ecran;
XEvent         evmt;
GC             ctx,ctx_xor;
unsigned long  cnoir, cblanc, cvert, cjaune, corange, cviolet, crouge, cbleu, cgris, ma_couleur, couleur_dessinateur;
Atom           XA_FORME, XA_COULEUR, XA_EPAISSEUR, XA_X1, XA_X2,XA_Y1,XA_Y2, XA_CHAT; 
char           chaine[200];
Atom           type_effectif_retour;
int            format_effectif_retour;
unsigned long  nb_lus_retour, nb_octets_restants_retour;
int            x1,x2,y1,y2;
int            bouton; 
forme          ma_forme, forme_dessinateur;
int            touche; 
int            mon_epaisseur, epaisseur_dessinateur; 
// Le curseur pour le texte dans le wchat
int            cury;

void Installer(char *serveur);
void PourButtonPress(XButtonPressedEvent *evmt);
void PourButtonRelease(XButtonReleasedEvent *evmt);
void PourMotionNotify(XPointerMovedEvent *evmt);
void PourKeyPress (XKeyPressedEvent *evmt);
void PourKeyRelease (XKeyReleasedEvent *evmt);
void PourExpose(XExposeEvent *evmt);
void PourPropertyNotify(XPropertyEvent *evmt);
void Draw(forme forme_dessin, int couleur_dessin, int epaisseur_dessin, GC ctx, int x1, int x2, int y1, int y2);

int main (int argc, char *argv[]){  /* la procedue main()                */
  Installer(NULL);
  for (;;){       /* la boucle d'evenements            */
    XNextEvent(dpy, &evmt);
    switch (evmt.type){
      /*?????????????????????????????????????/*/
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
  ma_forme = RECTANGLE;
  mon_epaisseur = 1;
  cury = 15;
  
  wprincipale = XCreateSimpleWindow(dpy, wracine, 0, 0, 500, 500, 0,  cnoir, cblanc);

  attr.override_redirect= True; 
  wmenu = XCreateWindow(dpy, wracine, 0, 0, 150, 250, 1,
			CopyFromParent, CopyFromParent, CopyFromParent,
			CWOverrideRedirect, &attr);

  wchat = XCreateSimpleWindow(dpy, wracine, 0, 0, 200, 300, 0, cnoir, cblanc);
  wtextinput = XCreateSimpleWindow(dpy,wchat,0,270,200,30,0, cnoir, cgris); 
  /* ?????????????????????????????????????*/
  
  ctx = XCreateGC(dpy, wprincipale, 0, NULL);
  XSetLineAttributes(dpy, ctx, mon_epaisseur, LineSolid, CapButt, JoinMiter);
  XSetForeground(dpy, ctx, cnoir);
  
  ctx_xor = XCreateGC(dpy, wprincipale, 0, NULL);
  XSetForeground(dpy, ctx_xor, cblanc);
  XSetFunction(dpy, ctx_xor, GXxor);


  /*??????????????????????????????????????????*/

  XStoreName(dpy,wprincipale, "Dessin");
  XStoreName(dpy,wchat, "Chat");
  XMapWindow(dpy,wprincipale);
  XMapWindow(dpy,wchat);
  XMapSubwindows(dpy,wchat); 
  XMapSubwindows(dpy,wmenu);

  XA_CHAT = XInternAtom(dpy, "CHAT", False);

  /* ????????????????????????? */
  
  XSetIOErrorHandler(XCloseDisplay);
}

void Draw(forme forme_dessin, int couleur_dessin, int epaisseur_dessin, GC ctx, int x1, int x2, int y1, int y2){
  /* ???????????????????????????? */
  switch(forme_dessin){
    case LIGNE : 
      /*????????????????*/
    case CERCLE :
      /*????????????????*/
    case CERCLEPLEIN :
      /*????????????????*/
    case RECTANGLE :
      /*????????????????*/
    case RECTANGLEPLEIN :
      /*????????????????*/
    default : ; 
    }
  /* ????????????????????????????? */
}
