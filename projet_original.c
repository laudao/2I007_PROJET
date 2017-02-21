
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
char           chaine[200];
Atom           type_effectif_retour;
int            format_effectif_retour;
unsigned long  nb_lus_retour, nb_octets_restants_retour;
int            xA,xB, yA,yB;
int            bouton; 
int			   nb_boutons_enfonces;
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
  /* ?????????????????????????????????????*/
  
  ctx = XCreateGC(dpy, wprincipale, 0, NULL);
  XSetLineAttributes(dpy, ctx, mon_epaisseur, LineSolid, CapButt, JoinMiter);
  XSetForeground(dpy, ctx, cnoir);
  
  ctx_xor = XCreateGC(dpy, wprincipale, 0, NULL);
  XSetForeground(dpy, ctx_xor, cblanc);
  XSetFunction(dpy, ctx_xor, GXxor);
  
  

   XSelectInput(dpy, wprincipale,
	       ButtonPressMask|ButtonReleaseMask|ButtonMotionMask|
	         OwnerGrabButtonMask);

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

void Draw(forme forme_dessin, int couleur_dessin, int epaisseur_dessin, GC ctx, int xA, int xB, int yA, int yB){
  XSetForeground(dpy, ctx_xor, couleur_dessin);
  XSetForeground(dpy, ctx, couleur_dessin);

  switch(forme_dessin){
    case LIGNE : 
  //    XDrawLine(dpy, wprincipale, ctx_xor, xA, yA, xB, yB);
      XDrawLine(dpy, wprincipale, ctx, xA, yA, xB, yB);
      break;
    case CERCLE :
    //  XDrawArc(dpy, wprincipale, ctx_xor, xA, yA, abs(xA-xB), abs(yA-yB),180 ,180);    
      XDrawArc(dpy, wprincipale, ctx , xA, yA, abs(xA-xB), abs(yA-yB), 0,360 *64);
      break;
    case CERCLEPLEIN :
    //  XFillArc(dpy, wprincipale, ctx_xor , xA, yA, abs(xA-xB), abs(yA-yB), 0,360 *64);    
      XFillArc(dpy, wprincipale, ctx , xA, yA, abs(xA-xB), abs(yA-yB), 0,360 *64);
      break;
    case RECTANGLE :
    
     // XDrawRectangle(dpy, wprincipale, ctx_xor,xA ,yA ,abs(xA-xB),abs(yA-yB));
      XDrawRectangle(dpy, wprincipale, ctx,xA ,yA ,abs(xA-xB),abs(yA-yB));
      break;
    case RECTANGLEPLEIN :
      //XFillRectangle(dpy, wprincipale, ctx_xor,xA ,yA ,abs(xA-xB),abs(yA-yB));
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
  ++nb_boutons_enfonces;
}

void PourButtonRelease (XButtonReleasedEvent *evmt) {
  --nb_boutons_enfonces;
  if (evmt->button == bouton) {
    switch (bouton) {
    case 3 :
    	XUnmapWindow(dpy, wmenu);
/*     		case wrouge:
     			ma_couleur=crouge;
				XClearWindow(dpy, wprincipale);
				break;
			case worange:
     			ma_couleur=corange;
				XClearWindow(dpy, wprincipale);
				break;
			case wvert:
     			ma_couleur=cvert;
				XClearWindow(dpy, wprincipale);
				break;
			case wbleu:
     			ma_couleur=cbleu;
				XClearWindow(dpy, wprincipale);
				break;
			case wjaune:
     			ma_couleur=cjaune;
				XClearWindow(dpy, wprincipale);
				break;
			case wviolet:
     			ma_couleur=cviolet;
				XClearWindow(dpy, wprincipale);
				break;
			case wnoir:
     			ma_couleur=cnoir;
				XClearWindow(dpy, wprincipale);
				break;
			case wrectanglevide:
     			ma_forme = RECTANGLE;
				XClearWindow(dpy, wprincipale);
				break;
			case wrectangleplein:
     			ma_forme = RECTANGLEPLEIN;
				XClearWindow(dpy, wprincipale);
				break;
			case wcercle:
     			ma_forme = CERCLE;
				XClearWindow(dpy, wprincipale);
				break;
			case wcercleplein:
     			ma_forme = CERCLEPLEIN;
				XClearWindow(dpy, wprincipale);
				break;
			case wligne:
     			ma_forme = LIGNE;
				XClearWindow(dpy, wprincipale);
				break;
			case wepaisseur1:
     			mon_epaisseur = 1;
				XClearWindow(dpy, wprincipale);
				break;
			case wepaisseur2:
     			mon_epaisseur = 2;
				XClearWindow(dpy, wprincipale);
				break;
			case wepaisseur3:
     			mon_epaisseur = 3;
				XClearWindow(dpy, wprincipale);
				break;
			case wepaisseur4:
     			mon_epaisseur = 4;
				XClearWindow(dpy, wprincipale);
				break;
      
      	}
*/
      break;
       
    case 1 :
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
    Draw(ma_forme,ma_couleur,mon_epaisseur,ctx_xor,xA, xB, yA,yB);
    xB = evmt->x;
    yB = evmt->y;
    /* dessiner un nouveau */
    Draw(ma_forme,ma_couleur,mon_epaisseur,ctx_xor,xA, xB, yA,yB);
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
