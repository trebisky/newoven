/* INtroduced 6-21-2019 when this was moved out of
 * IRAF and made standaone, along with ANSI conversion.
 * Tom Trebisky 6-21-2019
 */

/* Putting these here is kind of a quick kludge,
 * but harmless I think.
 */
#include <strings.h>
#include <string.h>
#include <time.h>

/* Functions in prints.c */
int prspace ( char *, int n );
int prstring ( char *, char *);
int prloc ( char *, LOC * );
int prlocr ( char *, LOC * );
int prloct ( char *, LOC * );
int prlocz ( char *, LOC * );
int praloc ( char *, LOC * );
int pralocr ( char *, LOC * );
int praloct ( char *, LOC * );
int pralocz ( char *, LOC * );
int prsped ( char *, TEMP );
int prtemp ( char *, TEMP );
int prtempd ( char *, BOOL, TEMP );
int prenable ( char *, BOOL );
int prenab ( char *, BOOL );
int prstatus ( char *, BOOL );
int prstat ( char *, BOOL );
int prignore ( char *, BOOL );
int prign ( char *, BOOL );
int prdeviant ( char *, BOOL );
int prclstatus ( char *, BOOL );
int prclstat ( char *, BOOL );
int prtctype ( char *, int );
int prijb ( char *, int );
int prserial ( char *, int );
int prbeam ( char *, int );
int prtwist ( char *, int );
int pridepth ( char *, int );
int prepsilon ( char *, int );
int prmaxload ( char *, int );
int prnnb ( char *, int n );
int prnoden ( char *, int );
int prvnumb ( char *, int );
int prvport ( char *, int );
int prnodel ( char *, int );
int pranodel ( char *, int );
int prnodew ( char *, p_node *);
int prstatusb ( char *, int, int );
int prtime ( char *, TIME );
int prrtime ( char *, TIME );
int prrefv ( char *, float );
int prpoly ( char *, float );
int prcoef ( char *, float );
int prprod ( char *, float );
int prapf ( char *, PFE, float, float, float );
int prapfv ( char *, PFE, float, float );
int prapfc ( char *, PFE, float, float );
int prapn ( char *, PFE, float );
int prapnc ( char *, PFE, float, float );
int prapt ( char *, PFE, float, float, float, float );
int prat ( char *, float, float, float, float );
int prah ( char *, int, float );
int prahv ( char *, int, float, float );
int prpercent ( char *, float  );
int prvolts ( char *, float  );
int pramps ( char *, float  );
int prheaters ( char *, float  );
int prkwatts ( char *, float  );
int prpower ( char *, POWER );
int prpowerd ( char *, BOOL, POWER );
int prdntx ( char *, DNTX  );
int prdnt0 ( char *, DNTX  );
int prd000 ( char *, DNTX  );
int prdsb ( char *, DSB  );
int prpfe ( char *, PFE  );
int prpf0 ( char *, PFE  );
int prp00 ( char *, PFE  );
int przone ( char *, int  );

/* Functions in scans.c */
int toggle ( BOOL * );
int toggleb ( int *, int );
int sctcloc ( char *, p_database *, DNTX * );
int scheloc ( char *, p_database *, PFE * );
int scloc ( char *, LOC * );
int sclocr ( char *, LOC * );
int scloct ( char *, LOC * );
int sclocz ( char *, LOC * );
int scaloc ( char *, LOC * );
int scalocr ( char *, LOC * );
int scaloct ( char *, LOC * );
int scalocz ( char *, LOC * );
int sctemp ( char *, TEMP * );
int scijb ( char *, int * );
int scserial ( char *, int * );
int scbeam ( char *, int * );
int sctwist ( char *, int * );
int scidepth ( char *, int * );
int scepsilon ( char *, int * );
int scmaxload ( char *, int * );
int scnoden ( char *, int * );
int scnnnb ( char *, int * );
int scnfnb ( char *, int * );
int scvnumb ( char *, int * );
int scvportd ( char *, int * );
int scvportp ( char *, int * );
int scpower ( char *, POWER * );
int sctime ( char *, TIME * );
int screfv ( char *, float * );
int scpoly ( char *, float * );
int sccoef ( char *, float * );
int scvolts ( char *, float * );
int scamps ( char *, float * );
int scstat ( char *, BOOL * );
int scenab ( char *, BOOL * );
int scclstat ( char *, BOOL * );
int scdel ( char * );
int scins ( char * );
int scnodel ( char *, int * );
int scanodel ( char *, int * );
int scdntx ( char *, DNTX * );
int scdsb ( char *, DSB * );
int scpfe ( char *, PFE * );
int sczone ( char *, int * );
int sczonea ( char *, int * );
int sctctype ( char *, int * );

/* shmalloc.c */
char *shmalloc(int nbytes, int noven, int ncomp, int readonly);
void shmfree(char *shmptr, int noven, int ncomp, int remove);

/* domenus.c */
int init_menus(void);
void free_menus(void);
int do_menus(int period, int offset);
int nmline(void);
// static int doline(int tty_fd, int tty, ITEM *ip, int itime, int nline, BOOL repaint, BOOL refresh);
// static int dostline(int tty_fd, int tty, int nlines, int top, int bot, int total, int autom);
int statusline(int tty_fd, int tty, int nlines, char *string);
int domgoto(int tty_fd, int tty, int nlines, int eswitch);
int domwritep(int tty_fd, int tty, int nlines);
int domeditp(int tty_fd, int tty, int nlines);
int domeditb(int tty_fd, int tty, int nlines);
int menugoto(char *id);
int domreturn(int tty_fd, int tty, int nlines);
int domenter(int tty_fd, int tty, int nlines, int ncols, int iswitch);
int dopcur(int tty_fd, int tty);
void updateitem(void);
int domkey(int tty_fd, int tty, int nlines, int mlines, int top, int bot, int ncols, int period, int offset, int oldstatus);
int mkey(int autom, int period, int offset);
// static int clgstr(char *param, char *outstr, int maxch);

/* console.c */
void con_init(void);
void con_close(void);
void con_size(int *ncols, int *nlines);
void con_clear(void);
void con_clear_line(void);
void con_clear_toeol (void);
void con_move(int col, int line);
void con_str(char *text);
void con_mvstr(int col, int line, char *text);
void con_bell(void);
void con_flush(void);
void con_debug(char *msg);
void con_raw(void);
void con_noraw(void);
void con_raw2(void);
int con_key(int period, int offset);
void con_get_param(char *param, char *val, int max);
void con_help(void);
void backup_(void);
int c_timcur(char *string, int num);

/* from errorreport.c */
int eunseen ( void );

/* from stale.c */
void stalereport ( void );

/* from cache.c */
int cache_parameter ( void );
int uncache_parameter ( void );
int flcache_parameter ( void );
int cache_biparameter ( void );
int uncache_biparameter ( void );
int flcache_biparameter ( void );

/* from context.c */
int push_context ( void );
int pop_context ( void );

/* database.c */
void db_bpwrite_disk(void);
void db_bpread_disk(void);
void db_bpwrite_oven(void);
void db_bpread_oven(void);
void db_pwrite_oven(void);
void db_bwrite_oven(void);
void db_pread_oven(void);
void db_bread_oven(void);
int db_dread_oven(void);
int db_eread_oven(void);
void ncompperror(int noven, int ncomp);

/* From automkey.c */
int automkey ( int, int );

/* cproto: init.c */
int init_globals(int noven, int ncomp);
void free_globals(void);
// int init_context(void);
// int free_context(void);
void init_databases(int noven, int ncomp, int readonly);
int init_database(int noven, int ncomp, int readonly);
void free_databases(int noven, int ncomp, int remove);
void free_database(int noven, int ncomp, int remove);

// int iamthepilot(void);

/* ipportrw.c */
int ipportwrite(int ip, int port, char *buf, int nbytes);
int ipportread(int ip, int port, char *buf, int nbytes);
int getovenip(int noven, int ncomp);
int tportwrite(int noven, int ncomp, int port, char *buf[], int nbytes);

/* errorreport.c */
// void timeoutreport(void);
void errorreport(void);
// void ereport(database *db, FILE *fp);
// void miscereport(database *db, FILE *fp);
// void adcereport(database *db, FILE *fp);
// void dcuereport(database *db, FILE *fp);
// void pripanelereport(database *db, FILE *fp);
// void panelereport(database *db, FILE *fp);
// void zoneereport(database *db, FILE *fp);
// void elogger(database *db, FILE *fp, int *count, int errnum, int addr);
void timestamp(char *string);
// int nelogline(void);
// int eunseen(void);
// void elogline(int n, char *s);
// int eloggoto(char *s);

/* ---------------------------------- */
/* ---------------------------------- */
/* ---------------------------------- */

/* temporary stuff from glue.c */

/* Ordinarily, these would come from:
 *     /u1/iraf/iraf_2.16.1/unix/hlib/libc/spp.h
 */
typedef long	XINT;
#define	XCHAR		short
#define	XEOF		(-2)

void hgkey_ ( int *, int *, int * );
void thgkey_ ( XINT * );
void flgkey_ ( int * );

void hclgst_ ( XCHAR *, XCHAR *, XINT * );

void hpageh_ ( void );
void backup_ ( void );

/* Stuff from IRAF sys/libc */

int c_ttyodes ( char * );
void c_xttysize ( int *, int* );
void c_ttyclear ( int, int );
void c_ttygoto ( int, int, int, int );
void c_ttyclearln ( int , int );
void c_ttycdes ( int );
void c_ttyputline ( int , int , char * , int );
void c_ttyctrl ( int , int , char *, int );
int c_timcur ( char *, int );
void c_strupk ( char *, XCHAR *, int );
void c_strpak ( XCHAR *, char *, int );

/* THE END */
