/*
 * File:   main.c
 * Author: BM
 *
 * Created on 26 Kasım 2014 Çarşamba, 12:59
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 *
 */

struct CELL
{
    char *anahtar;
    struct CELL *next;
};
int lookup(char *anahtar, struct CELL *l)
{
    if(l==NULL) return 0;
    else if(!strcmp(anahtar,l->anahtar)) return 1;
    else return lookup(anahtar,l->next);
}

int insert(char *anahtar, struct CELL **l)
{
    if(*l==NULL)
    {
        *l=(struct CELL*)malloc(sizeof(struct CELL));
        (*l)->anahtar=(char*)malloc((strlen(anahtar)+1)*sizeof(char));
        strcpy((*l)->anahtar,anahtar);
        (*l)->next=NULL;
        return 1;
    }
    else if(strcmp(anahtar,(*l)->anahtar))
        return insert(anahtar, &((*l)->next));
    else return 0;
}

void print_list(struct CELL *l)
{
    if(l!=NULL)
    {
        printf("%s ", l->anahtar);
        print_list(l->next);
    }

}


struct table_node
{
    int counter;
    struct CELL *header;
};

struct hash_tablosu
{
    struct table_node *tablo_basi;
    int tablo_uzunlugu;
    int multiplier;
};

unsigned hash(char *anahtar, int multiplier, int table_size)
{
    int i=0;
    unsigned int value=0;
    while(anahtar[i])
    {
        printf("anahtar=%d\n",anahtar[i]);
        value = (anahtar[i]+multiplier*value)% table_size;
        i++;
    }
    return value;
}
void listeyi_hash_tablosuna_donusturme(struct hash_tablosu *htable, struct CELL **liste_basi)
{


    if(*liste_basi==NULL) printf("Liste bos hasha dönüstürülemez...");


    struct CELL *eklenecek ;

    struct CELL *son_elemean;

    while (*liste_basi!=NULL)
    {

        eklenecek = *liste_basi;
        *liste_basi=(*liste_basi)->next;

        int hash_index=hash(eklenecek->anahtar,htable->multiplier,htable->tablo_uzunlugu);
        struct CELL *temp=(htable->tablo_basi+hash_index)->header;

        if((htable->tablo_basi+hash_index)->header==NULL)
        {

            (htable->tablo_basi+hash_index)->header=eklenecek;
            eklenecek->next=NULL;
            (htable->tablo_basi+hash_index)->counter++;
        }
        else
        {
            if(strcmp(eklenecek->anahtar,temp->anahtar))
            {
                while(temp->next!=NULL)
                {
                    temp=temp->next;
                }

                temp->next=eklenecek;
                eklenecek->next=NULL;
                (htable->tablo_basi+hash_index)->counter++;
            }
        }

    }




}

void initialize_hash_table(struct hash_tablosu **hash_table,
                           int multiplier, int table_size)
{
    int i;
    *hash_table=(struct hash_tablosu*)malloc(sizeof(struct hash_tablosu));
    if(*hash_table==NULL)
    {
        printf(" Hash tablosu icin yer ayrilamnadi... Hata!!!");
        exit(1);
    }
    (*hash_table)->tablo_basi=
        (struct table_node *)malloc(table_size*sizeof(struct table_node));
    if((*hash_table)->tablo_basi==NULL)
    {
        printf(" Hash tablosu icin yer ayrilamnadi... Hata!!!");
        exit(1);
    }
    (*hash_table)->tablo_uzunlugu= table_size;
    (*hash_table)->multiplier= multiplier;

    for(i=0; i<table_size; i++)
    {
        ((*hash_table)->tablo_basi+i)->counter = 0;
        ((*hash_table)->tablo_basi+i)->header = NULL;
    }
}


void print_hash_table(struct hash_tablosu *hash_table)
{
    if(hash_table)
    {
        int index;
        printf("----- HASH TABLOSU -----\n");
        for(index=0; index<hash_table->tablo_uzunlugu; index++)
        {
            printf("%5d : (%2d) ",index,(hash_table->tablo_basi +index)->counter);
            //  printf("%5d : (%2d) ",index,hash_table->tablo_basi[index].counter);
            print_list((hash_table->tablo_basi+index)->header);
            printf("\n");
        }
    }
    else printf("Hash Tablosu Bos ...\n");

}

int delete_dugum_liste(struct CELL **header,char *anahtar)
{
    struct CELL *simdiki,*onceki;
    simdiki=*header;
    while(simdiki && strcmp(simdiki->anahtar,anahtar))
    {
        onceki=simdiki;
        simdiki=simdiki->next;
    }
    if(!simdiki) return 0;
    if(simdiki==*header)
    {
        *header=(*header)->next;
    }
    else
    {
        onceki->next=simdiki->next;
    }
    free(simdiki->anahtar);
    free(simdiki);
    return 1;
}

void insert_hash_table(struct hash_tablosu *hash_table,
                       char *anahtar)
{
    int hash_index = hash(anahtar, hash_table->multiplier,
                          hash_table->tablo_uzunlugu);
    if(insert(anahtar,&((hash_table->tablo_basi +hash_index)->header)))
        (hash_table->tablo_basi +hash_index)->counter++;
}


void delete_hash_table(struct hash_tablosu *table, char *anahtar)
{
    int hash_index= hash(anahtar,table->multiplier, table->tablo_uzunlugu);
    if(delete_dugum_liste(&((table->tablo_basi +hash_index)->header),anahtar))
        (table->tablo_basi+hash_index)->counter--;
}

void liste_yok_et(struct CELL **liste_basi)
{
    struct CELL *onceki;
    while(*liste_basi)
    {
        onceki=*liste_basi;
        *liste_basi=(*liste_basi)->next;
        free(onceki->anahtar);
        free(onceki);
    }
}

void hash_table_yok_et(struct hash_tablosu **hash_table)
{
    int index;
    if(*hash_table)
    {
        for(index=0; index < (*hash_table)->tablo_uzunlugu; index++)
            liste_yok_et(&((*hash_table)->tablo_basi+index)->header);
        free((*hash_table)->tablo_basi);
        free(*hash_table);
    }
    *hash_table=NULL;

}

void hash_table_buyut(struct hash_tablosu **htable,
                      int multiplier, int tablo_uzunlugu)
{
    int i;
    struct CELL *liste_basi;
    struct hash_tablosu *yeni_tablo;

    if(*htable)
    {
        initialize_hash_table(&yeni_tablo, multiplier, tablo_uzunlugu);
        for(i=0; i<(*htable)->tablo_uzunlugu; i++)
        {
            liste_basi=((*htable)->tablo_basi+i)->header;
            while(liste_basi!=NULL)
            {
                insert_hash_table(yeni_tablo,liste_basi->anahtar);
                liste_basi=liste_basi->next;
            }
        }
        hash_table_yok_et(htable);
        *htable=yeni_tablo;
    }
}

void sil(struct hash_tablosu *htable, char *anahtar)
{
    struct CELL *header,*x;

    header=(htable->tablo_basi+hash(anahtar,htable->multiplier,htable->tablo_uzunlugu))->header;
    x=header;
    while(x!=NULL)
    {
        if(strcmp(x->anahtar,anahtar)==0)
        {
            x=header;
            break;
        }
        x=x->next;
    }
    if(x==header)
    {
        while(header!=NULL)
        {
            x=header;
            header=header->next;
            free(x->anahtar);
            free(x);
        }
        (htable->tablo_basi+hash(anahtar,htable->multiplier,htable->tablo_uzunlugu))->header=NULL;
    }
}

int aynimi(struct hash_tablosu *h1, struct hash_tablosu *h2){
//iki hash table verilerinin karşılaştırmasını  yapar tablo boyutları farklı olabilir

if(h1==NULL || h2==NULL) return 1;
if(h1!=NULL && h2!=NULL) {

    int h1_eleman=0;
    int h2_eleman=0;
    int eslesen_eleman=0;

    int j,k,l,i;
    for (i=0;i<h1->tablo_uzunlugu;i++){
        h1_eleman +=h1->tablo_basi[i].counter;
    }
    for(i=0;i<h2->tablo_uzunlugu;i++){
        h2_eleman+=h2->tablo_basi[i].counter;

    }
    if(h1_eleman!= h2_eleman) return 0;

    for(i=0;i<h1->tablo_uzunlugu;i++){
        if(h1->tablo_basi[i].counter>0){
            struct CELL *x=h1->tablo_basi[i].header;
            while(x!=NULL){
                for(j=0;j<h2->tablo_uzunlugu;j++){
                    if(h2->tablo_basi[j].counter>0){
                        struct CELL *y=h2->tablo_basi[j].header;
                        while(y!=NULL){
                            if(!strcmp(x->anahtar,y->anahtar)){
                                eslesen_eleman++;
                            }
                            y=y->next;
                        }
                    }
                }
                x=x->next;
            }
        }
    }
    if(eslesen_eleman==h1_eleman) return 1;
    else return 0;

}
else return 0;
}

struct CELL* hucre_olustur(char *icerik)
{
    struct CELL *a;
    a=(struct CELL*)malloc(sizeof(struct CELL));
    if(a==NULL)
    {
        printf("Heap alaninda yer ayrilamadi ...\n");
        exit(1);
    }
    a->anahtar=icerik;
    a->next=NULL;
    return a;
}
void liste_yaz(struct CELL *liste_basi)
{
    while(liste_basi!=NULL)
    {
        printf("%4s ",liste_basi->anahtar);
        liste_basi=liste_basi->next;
    }
    printf("\n");
}
void liste_basina_ekle(char *icerik,struct CELL **liste_basi)
{
    struct CELL* a=hucre_olustur(icerik);
    a->next=*liste_basi;
    *liste_basi=a;
}

int main(int argc, char** argv)
{
  
    struct hash_tablosu *htable=NULL;
    struct CELL *liste=NULL;
    struct hash_tablosu *htable2=NULL;
    struct CELL *liste2=NULL;
    int ayni;

    initialize_hash_table(&htable,3,5);
    initialize_hash_table(&htable2,3,6);
    liste_basina_ekle("gundogdu",&liste);
    liste_basina_ekle("baklava",&liste);

    liste_basina_ekle("Trabzonspor",&liste);
    liste_basina_ekle("bedava",&liste);
    liste_basina_ekle("yasasin",&liste);
    liste_basina_ekle("cozum",&liste);
    liste_basina_ekle("zzz",&liste);


    liste_basina_ekle("dembaba",&liste);
    liste_basina_ekle("cardoza",&liste);
    liste_basina_ekle("besiktas",&liste);
    liste_basina_ekle("aaaaaa",&liste);
    liste_basina_ekle("bbbbb",&liste);

    // liste_yaz(liste);
    // print_list(liste);
   //listeyi_hash_tablosuna_donusturme(htable,&liste);
   // print_hash_table(htable);
   // printf("%4s",liste->anahtar);
     insert_hash_table(htable,"bbbbb");
     insert_hash_table(htable,"aaaaaa");
     insert_hash_table(htable,"Trabzonspor");
     insert_hash_table(htable,"kadayif");
     insert_hash_table(htable,"gundogdu");
     insert_hash_table(htable,"besiktas");
     insert_hash_table(htable,"baklava");
     insert_hash_table(htable,"dembaba");
     insert_hash_table(htable,"cardoza");
     // print_hash_table(htable);

     insert_hash_table(htable2,"bbbbb");
     insert_hash_table(htable2,"aaaaaa");
     insert_hash_table(htable2,"Trabzonspor");
     insert_hash_table(htable2,"kadayif");
     insert_hash_table(htable2,"gundogdu");
     insert_hash_table(htable2,"besiktas");
     insert_hash_table(htable2,"baklava");
     insert_hash_table(htable2,"dembaba");
     insert_hash_table(htable2,"cardoza");
     insert_hash_table(htable2,"cardozasc");
     insert_hash_table(htable2,"cardozaaaa");
      //print_hash_table(htable2);
      ayni=aynimi(htable,htable2);
      printf("ayni =%d",ayni);
//
//     hash_table_buyut(&htable,17,19);
//     print_hash_table(htable);
//
//     insert_hash_table(htable,"ankara");
//     insert_hash_table(htable,"Ankara");
//     print_hash_table(htable);
//
//     sil(htable,"kadayif");
//      sil(htable,"Ankara");
//     print_hash_table(htable);
    return (EXIT_SUCCESS);
}



