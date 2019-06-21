#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	CH_REPEAT	'!'
#define	CH_NG		'$'
#define	CH_GO		'%'
#define	CH_NO		'$'
#define	CH_OUT		'%'
#define	CH_IN		'?'
#define	CH_TOG		'&'

#define	TX_NX	"\n%s%s ()\n{\n\tint\tn = 0;\n\n\treturn (n);\n}\n"
#define	TX_GO	"\n%s%sg (%s)\n%s{\n\tint\tt = 0;\n\n%s"
#define	TX_TOG	"\n%s%st (%s)\n%s{\n\tint\tt = 0;\n\n%s"
#define	TX_IN	"\n%s%si (%ss)\n%schar\t*s;\n{\n\tint\tt = 0;\n\n%s"
#define	TX_OUT	"\n%s%so (%ss)\n%schar\t*s;\n{\n\tint\tt = 0;\n\n%s"
#define	TX_P	( (repeat) ? "n" : "" )
#define	TX_Q	( (repeat) ? "n, " : "" )
#define	TX_R	( (repeat) ? "int\tn;\n" : "" )
#define	TX_Z	"\treturn (t);\n}\n"

void makemenus ( char *, FILE *, FILE *, FILE *, FILE * );

// #define MAX_PATH 32
#define MAX_PATH 128
/* makemenus - generate C code from the menu descriptor files
 */
int
main ( int argc, char *argv[] )
{
	int	argi;
	FILE	*cfp, *efp, *mfp, *sfp;
	char	*fname;
	char	sname[32];
	char	*id;

	if ((cfp = fopen ("../menusm/menus.vc", "w")) == NULL)
	    exit (1);
	if ((efp = fopen ("../menusm/menus.ext", "w")) == NULL)
	    exit (2);

	for (argi = 1; argi < argc; argi++) {
	    fname = argv[argi];
	    for (id = fname + strlen (fname)-1; id > fname; id--)
		if (*(id-1) == '/')
		    break;
	    sprintf (sname, "%s.tc", id);
	    if ((mfp = fopen (fname, "r")) == NULL)
		break;
	    if ((sfp = fopen (sname, "w")) == NULL)
		break;

	    makemenus (id, mfp, cfp, efp, sfp);

	    fclose (mfp);
	    fclose (sfp);
	}

	fclose (cfp);
	// fclose (mfp);
	fclose (efp);

	exit (0);
}

// #define MAX_LINE	82
#define MAX_LINE	256
/* The first file is always "aa", so ...
 *  id  is "aa"
 *  mfp is ../menus/aa (we read this)
 *  cfp is ../menusm/menus.vc
 *  efp is ../menusm/menus.ext
 *  sfp is aa.tc
 */
void
makemenus ( char *id, FILE *mfp, FILE *cfp, FILE *efp, FILE *sfp )
{
	char	line[MAX_LINE];
	char	Line[MAX_LINE];	/* previous line	*/
	char	ename[10];
	int	lineno = 0;
	int	eof = 0;
	char	*lp, *ep, *fp;
	int	repeat;

	// printf ( "Processing %s\n", id );

	fprintf (cfp, "#include \"menus.m\"\n");
	fprintf (cfp, "\tmp->id = \"%s\";\n", id);

	fprintf (sfp, "#include \"../oven.h\"\n");
	fprintf (sfp, "#include \"../menus.h\"\n");
	fprintf (sfp, "#include \"../context.h\"\n");
	fprintf (sfp, "#include \"../global.h\"\n");

	line[0] = 0;
	while (!eof) {
	    strcpy (Line, line);
	    if (!(eof = (fgets (line, sizeof(line), mfp) == NULL))) {
	        if (strlen (line) > 0 && line[strlen (line) - 1] == '\n')
		    line[strlen (line) - 1] = 0;
	        if (lineno++ == 0)
		    continue;
	        repeat = (line[0] == CH_REPEAT);
	    }
	    if (Line[0] == CH_REPEAT) {
		strcpy (ename, Line+1);
		fprintf (cfp, "\tip->ntimes = %s%s;\n", id, ename);
		fprintf (efp, "extern int\t%s%s();\n", id, ename);
		fprintf (sfp, TX_NX, id, ename);
		continue;
	    }
	    fprintf (cfp, "#include \"menus.i\"\n");

	    for (lp = Line; *lp; lp++) {
		if (*lp == CH_NG || *lp ==CH_GO) {
		    for (ep=ename, fp=lp+1; *fp != ' '; *ep++ = *fp++)
			;
		    *ep = 0;
		    if (*lp == CH_GO) {
			fprintf (cfp, "\tip->gfunc = %s%sg;\n", id, ename);
			fprintf (efp, "extern int\t%s%sg();\n", id, ename);
			fprintf (sfp, TX_GO, id, ename, TX_P, TX_R, TX_Z);
		    }
		    for (; *fp; fp++) {
			if (*fp == CH_NO) {
			    break;
			}
			if (*fp == CH_IN) {
			    fprintf (cfp, "\tip->ifunc = %s%si;\n", id, ename);
			    fprintf (efp, "extern int\t%s%si();\n", id, ename);
			    fprintf (sfp, TX_IN, id, ename, TX_Q, TX_R, TX_Z);
			}
			if (*fp == CH_TOG) {
			    fprintf (cfp, "\tip->tfunc = %s%st;\n", id, ename);
			    fprintf (efp, "extern int\t%s%st();\n", id, ename);
			    fprintf (sfp, TX_TOG, id, ename, TX_P, TX_R, TX_Z);
			}
			if (*fp == CH_OUT || *fp == CH_IN || *fp == CH_TOG) {
			    fprintf (cfp, "\tip->ofunc = %s%so;\n", id, ename);
			    fprintf (efp, "extern int\t%s%so();\n", id, ename);
			    fprintf (sfp, TX_OUT, id, ename, TX_Q, TX_R, TX_Z);
			    break;
			}
		    }
		    fprintf (cfp, "\tip->func_start = %d;\n", lp-Line);
		    fprintf (cfp, "\tip->func_end   = %d;\n", fp-Line);
		    for (ep = lp; ep <= fp; *ep++ = ' ')
			;
		    break;
		}
	    }

	    for (lp = Line; *lp; lp++) {
		if (*lp != ' ') {
		    for (fp = Line + strlen (Line) - 1; *fp; fp--) {
			if (*fp != ' ') {
			    break;
			}
		    }
		    fprintf (cfp, "\tip->text_start = %d;\n", lp-Line);
		    fprintf (cfp, "\tip->text_end   = %d;\n", fp-Line);
		    *(fp+1) = 0;
		    fprintf (cfp, "\tip->text = \"%s\";\n", lp);
		    break;
		}
	    }
	}
}

/* THE END */
