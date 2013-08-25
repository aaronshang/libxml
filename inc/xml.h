#ifndef __LIB_XML__H
#define __LIB_XML__H

#include <stddef.h>
#define _BSD_SOURCE
#include <stdio.h>
#undef _BSD_SOURCE

#define XML_MAX_INDENT_LEVEL 1024

#define XML_LEVEL_TRIVIAL 	0
#define XML_LEVEL_NORMAL 	1
#define XML_LEVEL_IMPORTANT 2
#define XML_LEVEL_CRITICAL 	3
#define XML_LEVEL_NONE		4

#ifndef XML_DEBUG_LEVEL
	#define XML_DEBUG_LEVEL XML_LEVEL_TRIVIAL
#endif

int isempty ( const char * const str);

struct 						s_xmllist;
struct						s_xmlattriblist;

typedef struct				s_xmlattrib
{
	char			*		name;
	char			*		value;
	struct s_xmlattrib *	next;
	struct s_xmlattrib *	prev;
	struct s_xmlattriblist*	list;
}							t_xmlattrib;

typedef struct				s_xmlattriblist
{
	t_xmlattrib			*	begin,
						*	end;
	size_t					size;
	struct s_xmlnode	*	parent;
}							t_xmlattriblist;

typedef struct  	s_xmlnode
{
	char				* name,
						* value;
	t_xmlattriblist		* attribs;
	struct s_xmllist	* childs; 
	struct s_xmlnode	* parent, 
						* next, 
						* prev;
}					t_xmlnode;

typedef struct		s_xmllist
{	
	t_xmlnode 	* 	begin,
				* 	end;
	size_t			size;
	t_xmlnode	* 	parent;
}					t_xmllist;

typedef struct		s_xmldocument
{
	char			version[10];
	char			standalone[5];
	char			encoding[10];
	
	t_xmlnode	*	root;
}					t_xmldocument;

t_xmldocument	*	xmldoc_alloc (const char * const encoding, const char * const version, const char * const standalone,const char * const root_name, const char * const root_value);
t_xmldocument	*	xmldoc_init (t_xmldocument * d, const char * const encoding, const char * const version, const char * const standalone,const char * const root_name, const char * const root_value);
t_xmldocument	*	xmldoc_print (FILE *stream, t_xmldocument * d, char indent_char, size_t indent);
t_xmldocument	*	xmldoc_clear (t_xmldocument * d);
void				xmldoc_free (t_xmldocument * d);

t_xmlattrib	*		xmlattrib_alloc (t_xmlattriblist * list, const char * const name, const char * const value);
t_xmlattrib	*		xmlattrib_init (t_xmlattrib* attrib, t_xmlattriblist * list, const char * const name, const char * const value);
void				xmlattrib_free (t_xmlattrib* attrib);

t_xmlattriblist	*	xmlattriblist_alloc (t_xmlnode* parent);
t_xmlattriblist	*	xmlattriblist_init (t_xmlattriblist* l, t_xmlnode* parent);
t_xmlattriblist	*	xmlattriblist_append (t_xmlattriblist* l, t_xmlattrib *a);
t_xmlattriblist	*	xmlattriblist_prepend (t_xmlattriblist* l, t_xmlattrib *a);
t_xmlattriblist	*	xmlattriblist_clear (t_xmlattriblist* l);
void				xmlattriblist_free (t_xmlattriblist* l);

t_xmllist * 		xmllist_alloc(t_xmlnode * parent);
t_xmllist * 		xmllist_init(t_xmllist *,t_xmlnode * parent);
t_xmlnode * 		xmllist_append(t_xmllist *, t_xmlnode * );
t_xmlnode * 		xmllist_prepend(t_xmllist *, t_xmlnode * );
t_xmllist * 		xmllist_clear(t_xmllist *);
void				xmllist_free(t_xmllist *);

t_xmlnode*			xmlnode_clear (t_xmlnode*n);
size_t 				xmlnode_level (t_xmlnode*n);
char		*		xmlnode_level_indent(t_xmlnode*n, char indent_char, char * buff, size_t max_len);
void 				xmlnode_print (FILE* stream, t_xmlnode *n, char indent_char, size_t indent);

t_xmlnode * 		xmlnode_alloc( 	t_xmlnode *parent,
									const char * const name,
									const char * const value );
							
t_xmlnode * 		xmlnode_init ( 	t_xmlnode *node,
									t_xmlnode *parent,
									t_xmlnode *next,
									t_xmlnode *prev,
									const char * const name,
									const char * const value );

void 				xmlnode_free (t_xmlnode * n);

/************************************
 * 			Parsing funcs
 ************************************/
 
#endif
