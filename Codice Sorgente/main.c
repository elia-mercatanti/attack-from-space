#include <allegro.h>
#include <stdio.h>
#include "dati.h"
#include "discovolante.h"
#include "esplosione.h"
#include "missile.h"

DATAFILE *dati,*discovolante,*esplosione,*missil;

BITMAP *buf,*scrolling;

int alt=1080,larg=1920,xp,yp,velox=10,limx=260,limy=90,molt,xscroll,vel_f=5,xc,yc,xconto,yconto,vel_n=10;
int sini=40,morto,larg_g=251,larg_n=210,alt_g=83,alt_n=113,t_esplgioc,fuoco,xmiss,ymiss,mortoc,vel_m=30,tempesplcattivo;
int contapunti,vite,fine,go,suono1,suono2,suono3,suono4,discoani=0,esploani=0,missani=0,missinv=0;;
char score[80];

PALETTE colori;

void doppiobuffering()
{
    vsync();
    blit(buf, screen, 0, 0, 0, 0, larg, alt);
    clear(buf);
}

void giocatore()
{
    if (morto==1) {
        draw_sprite(buf, dati[PLAYER].dat, xp, yp);
    }
}    

void pulsanti()
{
    if (morto==1){
        if (key[KEY_LEFT]) xp=xp-velox; if (xp<=0) xp=0; 
        if (key[KEY_RIGHT]) xp=xp+velox; if (xp>=larg-limx) xp=larg-limx; 
        if (key[KEY_UP]) yp=yp-velox; if (yp<=5) yp=5; 
        if (key[KEY_DOWN]) yp=yp+velox; if (yp>=alt-limy) yp=alt-limy; 
        if (key[KEY_SPACE]) fuoco=1;
    }    
}

void fondale()
{
    blit(dati[SOLENUVOLE].dat, buf, 0, 0, 0, 0, larg, alt);
    draw_sprite(buf, scrolling, xscroll,0);
    xscroll=xscroll-vel_f;
    if (xscroll<-1919) {
        xscroll = 0;
    }
}

void nemico() 
{
    if (mortoc==1) {
        draw_sprite(buf, discovolante[discoani].dat, xc, yc);
        discoani++;
        if (discoani==9) discoani=0;
    }    
}

void movimentonemico() 
{
    if (mortoc==1) {
    if (yconto==1) {
        yc=yc-vel_n;
        if (yc<=15) {
            yconto=4;
        }
    }
    if (yconto==2) {
        yc=yc+vel_n;
        if (yc>=950) {
            yconto=3;
        }
    }
    if (yconto==3) {
        xc=xc-vel_n;
        xconto=xconto+1;
        if (xconto>=sini) {
            xconto=0; yconto=1;
        }
    } 
    if (yconto==4) {
        xc=xc-vel_n;
        xconto=xconto+1;
        if (xconto>=sini) {
            xconto=0; yconto=2;
        }
    }      
    if (xc<=-210) {
        xc=1900;
        yc=rand()%950+5;
        yconto=3;
    }
    if (xc<=200) {
        xc=xc-vel_n;
    }
    }    
}
 
void collisione() 
{
    if (mortoc==1){
    if (((xp+larg_g)>=xc) && (xp<=(xc+larg_n))) {
        if (((yp+alt_g)>=yc) && (yp<=(yc+alt_n))) {
            morto=0;
            if (suono1==0) {
                play_sample(dati[VITAMENO].dat, 255,128,1000, FALSE);
                suono1=1;
            }    
        }
        
    }  
    }      
}
           
void esplgioc() 
{
    
    if (morto==0) {
        if (t_esplgioc<=20) {
            t_esplgioc++;
            draw_sprite(buf,esplosione[esploani].dat, xp, yp);
            esploani++;
            if (esploani==9) esploani=0;
        }
        else {
            xp=1; yp=310;
            morto=1;
            t_esplgioc=0;
            vite--;
            suono1=0;
        }       
    }        
}

void missile()
{
    if (fuoco==0) {
        xmiss=xp+150; 
        ymiss=yp+40;
    }    
    if (fuoco==1) {
        draw_sprite(buf, missil[missani].dat, xmiss, ymiss);
        if (missinv==0) missani++;
        if (missani==5) missinv=1;
        if (missinv==1) missani--;
        if (missani==0) missinv=0;
        xmiss=xmiss+vel_m;
        if (suono2==0) {
            play_sample(dati[FUOCO].dat, 255,128,1000, FALSE);
            suono2=1;
        }    
    }    
    if (xmiss>=1920) 
        fuoco=0;
        suono2=0;
}
 
void cattivocolpito() 
{
    if (fuoco==1){
    if (((xmiss+50)>=xc) && (xmiss<=(xc+larg_n))) {
        if (((ymiss+26)>=yc) && (ymiss<=(yc+alt_n))) {
            if (suono4==0) {
                play_sample(dati[GAMEOVER].dat, 255,128,1000, FALSE);
                suono4=1;
            }    
            mortoc=0;
            fuoco=0;
            contapunti=contapunti+10;
            suono2=0;
        }    
    }
    }    
}            
               
void esplcattivo() 
{
    if (mortoc==0) {
        if (tempesplcattivo<=20) {
            tempesplcattivo++;
            draw_sprite(buf,esplosione[esploani].dat, xc, yc);
            esploani++;
            if (esploani==9) esploani=0;
        }
        else 
        {
            xc=1900; yc=rand()%950+5;
            tempesplcattivo=0;
            mortoc=1;
            suono4=0;
        }    
    }
}       

void puntivite()
{
    sprintf(score, "PUNTI: %ld VITE: %ld", (long)contapunti, (long)vite);
    textout(buf, font, score, 900, 5, 200000000); 
}      
 
void gameover() 
{
    if (vite==0) {
        if (fine<=200) {
            textout(buf, font, "GAME OVER", 900, 540, 200000000);
            fine++;
            if (suono3==0) {
                play_sample(dati[GAMEOVER].dat, 255,128,1000, FALSE);
                suono3=1;
            }    
        }
        else {
            t_esplgioc=0;
            tempesplcattivo=0;
            morto=1;
            xp=10; yp=30;
            xc=360; yc=10;
            yconto=3; xconto=0;
            fuoco=0;
            xmiss=0; ymiss=0;
            mortoc=1;
            contapunti=0;
            vite=3;
            fine=0;
            go=1;
            suono1=0; suono2=0; suono3=0, suono4=0;
        }    
        
    }
}            

void presentazione() 
{
    if (go==1) {
        while (!key[KEY_ENTER]) {
            blit(dati[PRESENTAZIONE].dat, screen, 0, 0, 0, 0, larg, alt);
        }
    }
    go=0;
}            
        
    
            
int main()
{
    srand( (unsigned)time( NULL ) );
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_palette(colori);
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);
    set_volume(800,800);
    set_gfx_mode(GFX_AUTODETECT, larg, alt, 0, 0);
    buf=create_bitmap(larg,alt);
    clear(buf);
    dati = load_datafile("dati.dat");
    discovolante = load_datafile("discovolante.dat");
    esplosione = load_datafile("esplosione.dat");
    missil = load_datafile("missile.dat");
    scrolling = create_bitmap(larg*2, alt);
    clear(scrolling);
    for (molt=0; molt<=larg*2; molt=molt+larg) {
        blit(dati[FONDALE].dat, scrolling, 0, 0, molt, 0, larg, alt);
    } 
    t_esplgioc=0;
    tempesplcattivo=0;
    morto=1; mortoc=1; 
    xp=1,yp=310;
    xc=1900; yc=rand()%950+5;
    yconto=3; xconto=0;
    fuoco=0; xmiss=0; ymiss=0;
    contapunti=0;
    vite=4; fine=0; go=1;
    suono1=0; suono2=0; suono3=0, suono4=0;
    play_midi(dati[MUSICAGIOCO].dat, TRUE);
    while (!key[KEY_ESC]){
        doppiobuffering();
        fondale();
        giocatore();
        pulsanti();
        nemico();
        movimentonemico();
        collisione();
        esplgioc();
        missile();
        cattivocolpito();
        esplcattivo();
        puntivite();
        gameover();
        presentazione();
    }
    destroy_bitmap(buf);
    destroy_bitmap(scrolling);
    
}    
END_OF_MAIN();       
