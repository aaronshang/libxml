#include "xml.h"
#include <stdlib.h>
#include <string.h>

#define XML_DBGLEVEL_NODE XML_LEVEL_TRIVIAL
#define XML_DBGLEVEL_ATTRIB XML_LEVEL_TRIVIAL
#define XML_DBGLEVEL_NODELIST XML_LEVEL_TRIVIAL
#define XML_DBGLEVEL_ATTRIBLIST XML_LEVEL_TRIVIAL
#define XML_DBGLEVEL_DOCUMENT XML_LEVEL_TRIVIAL

#ifndef NDEBUG
#	define XML_DBG_BLOCK_BEGIN(level) if(level>=XML_DEBUG_LEVEL){
#	define XML_DBG_BLOCK_END }else (void)0;
#	define DBGMSG(msg) printf ("[LOG] %s\n", msg);
#	define DBGMSGL(msg,level) if(level>=XML_DEBUG_LEVEL) printf ("[LOG:%i] %s\n", level, msg)
#	define DBGMSGN(msg,name) printf ("[LOG] %s %s\n", msg, name)
#	define DBGMSGNL(msg,name,level) if(level>=XML_DEBUG_LEVEL) printf ("[LOG:%i] %s %s\n", level, msg, name)
#else
#	define XML_DBG_BLOCK_BEGIN(level)
#	define XML_DBG_BLOCK_END
#	define DBGMSG(msg)
#	define DBGMSGL(msg, level)
#	define DBGMSGN(msg,name)
#	define DBGMSGNL(msg,name,level)
#endif
    
#define safe_strdup(str) strdup (str?str:"");
#define safe_free(p) if(p){DBGMSGL ("Freeing " #p, XML_LEVEL_TRIVIAL); free (p); p=NULL;}
#define struct_free(s, membr) if(s){safe_free( s->membr ); s->membr=NULL;}

int isempty (const char * const str)
{
	return ((str&&(strlen(str)==0))?1:0);
}

t_xmlnode * xmlnode_alloc( 	t_xmlnode *parent,
							const char * const name,
							const char * const value )
{
	t_xmlnode * ret;
	if (!name ||
		!value) 
		return NULL;
	DBGMSGNL("Allocating new xmlnode of type ", isempty(name)?"<undefined>":name, XML_LEVEL_TRIVIAL);
	ret = (t_xmlnode*) malloc (sizeof (t_xmlnode));
	ret->childs = NULL;
	ret->attribs = NULL;
	xmlnode_init (ret, parent, NULL, NULL, name, value);
	return ret;
}

t_xmlnode * xmlnode_update (
	t_xmlnode * n, 
	const char * const name, 
	const char * const value)
{
	if (!n)
		return n;
	
	if (name)
	{
		safe_free (n->name);
		n->name = strdup (name);
	}
	
	if (value)
	{
		safe_free (n->value);
		n->value = strdup (value);
	}	
	
	return n;
}

t_xmllist* xmlnode_parse (t_xmllist* l, const char * const str)
{
	const char * p = str;
	if (!l)
		return l;
	while (p && *p != '\0')
	{
		if(*p == '<')
			printf("New node\n");
		p++;
	}
	return l;
}

t_xmlnode * xmlnode_init ( 	t_xmlnode *node,
							t_xmlnode *parent,
							t_xmlnode *next,
							t_xmlnode *prev,
							const char * const name,
							const char * const value )
{
	const char * const none = "";
	const char * const undf = "<undefined>";
	
	if (!node)
		return node;

	XML_DBG_BLOCK_BEGIN (XML_LEVEL_TRIVIAL)
		printf ("Initialising xmlnode: %s\n", isempty(name)?undf:name);
		printf ("\tvalue: %s\n", isempty(value)?undf:value);
	XML_DBG_BLOCK_END
	
	node->name = strdup (name?name:none);
	node->value = strdup (value?value:none);
	
	node->next = next;
	node->prev = prev;
	node->parent = parent;
	
	node->childs = xmllist_alloc (parent);
	node->attribs = xmlattriblist_alloc (node);
	return node;
}

void xmlnode_free (t_xmlnode * n)
{
	t_xmllist *c;
	if (!n)
		return ;
	c = n->childs;
	struct_free (n, name);
	struct_free (n, value);
	xmllist_free (c);
	xmlattriblist_free (n->attribs);
	if (n->prev)
		n->prev->next = n->next;
	if (n->next)
		n->next->prev = n->prev;
	if (n->parent&&n->parent->childs)
	{
		if (n->parent->childs->begin == n )
		{
			if (n->parent->childs->begin->next)
				n->parent->childs->begin = n->parent->childs->begin->next;
			else
				n->parent->childs->begin = NULL;
		}
		else if (n->parent->childs->end == n )
		{
			if (n->parent->childs->end->prev)
				n->parent->childs->end = n->parent->childs->begin->prev;
			else
				n->parent->childs->end = NULL;
		}
	}
	n->next = NULL;
	n->prev = NULL;
	safe_free (n);
}

/***********************
 * 		LIST
 ***********************/

t_xmlnode * xmllist_append(t_xmllist *l, t_xmlnode *n )
{
	if (!l||!n) return NULL;
	
	DBGMSGNL("Appending xmlnode:", n->name, XML_LEVEL_TRIVIAL);
	
	if (!l->begin)
	{
		l->begin=l->end=n;
	}
	else
	{
		l->end->next = n;
		n->prev = l->end;
		l->end = n;
	}
	n->parent = l->parent;
	l->size ++;
	return n;
}
t_xmlnode * xmllist_prepend(t_xmllist *l, t_xmlnode *n )
{
	if (!l||!n) return NULL;
	
	DBGMSGNL("Appending xmlnode:", n->name, XML_LEVEL_TRIVIAL);
	
	if (!l->begin)
	{
		l->begin=l->end=n;
	}
	else
	{
		l->begin->prev = n;
		n->next = l->begin;
		l->begin = n;
	}
	n->parent = l->parent;
	l->size ++;
	return n;
}

t_xmllist * xmllist_alloc( t_xmlnode * parent )
{
	DBGMSGL("Allocating new xmllist", XML_LEVEL_TRIVIAL);
	return xmllist_init((t_xmllist*)malloc (sizeof(t_xmllist)), parent);
}
t_xmllist * xmllist_init(t_xmllist *l, t_xmlnode * parent){
	DBGMSGL("Initialising xmllist", XML_LEVEL_TRIVIAL);
	if (!l) return l;
	l->begin=l->end=NULL;
	l->parent=parent;
	l->size=0;
	return l;
}
t_xmllist * xmllist_clear(t_xmllist *l)
{
	t_xmlnode * n, * nn;
	if (!l) return l;
	DBGMSGL("Clearing xmllist", XML_LEVEL_TRIVIAL);
	n = l->begin;
	while (n)
	{
		nn = n->next;
		xmlnode_free (n);
		n = nn;
	}
	l->size = 0;
	l->begin = NULL;
	l->end = NULL;
	return l;
}
void		xmllist_free(t_xmllist *l)
{
	if (!l) return ;
	DBGMSGL("Freeing xmllist", XML_LEVEL_TRIVIAL);
	xmllist_clear (l);
	safe_free (l);
}

t_xmlnode	*	xmlnode_clear (t_xmlnode * d)
{
	if (!d)
		return d;
	xmllist_clear (d->childs);
	xmlattriblist_clear (d->attribs);
	strcpy( d->name, "" );
	strcpy( d->value, "" );
	return d;
}

size_t 		xmlnode_level (t_xmlnode*n)
{
	t_xmlnode *c;
	size_t ret;
	c = n;
	ret = 0;
	while (c)
	{
		ret++;
		c = c->parent;
	}
	return ret;
}

char		*xmlnode_level_indent(t_xmlnode*n, char indent_char, char * buff, size_t max_len)
{
	t_xmlnode *c;
	size_t i;
	if (!buff)
		return buff;
	memset (buff, '\0', max_len);
	c = n;
	i = 0;
	while (c && i<max_len)
	{
		buff[i] = indent_char;
		i++;
		c = c->parent;
	}
	buff[i] = '\0';
	return buff;
}

char * make_indent_string ( size_t indent, char indent_char, char * buf, size_t len )
{
	size_t i, max_;
	if (!buf) 
		return NULL;
	max_ = ((indent > len) ? len : indent);
	for (i=0; i<max_; i++)
		buf[i] = indent_char;
	buf[i] = '\0';
	return buf;
}

void 		xmlnode_print (FILE* stream, t_xmlnode *n, char fill_char, size_t indent)
{
	static char cur_lvl[XML_MAX_INDENT_LEVEL+1];
	int short_tag;
	t_xmlattrib *a;
	if (!n)
		return ;
	short_tag=0;
	
	fprintf (stream, 
			"%s<%s",
			make_indent_string (indent,fill_char,cur_lvl,XML_MAX_INDENT_LEVEL),
			n->name);
	if (isempty(n->value)&&!n->childs->begin)
		short_tag = 1;

	a = n->attribs?n->attribs->begin:NULL;
	while (a)
	{
		fprintf (stream, 
			" %s=\"%s\"", a->name, a->value);
		a = a->next;
	}
	/*if (!isempty(n->name))
		printf (" id=\"%s\"", n->name);
	*/	
	if (!short_tag)
		fprintf (stream, 
			">\n");
	else
		fprintf (stream, 
			" />\n");
			
	if (!isempty(n->value))
		fprintf (stream, 
			"%s%s\n", 
			make_indent_string (indent+1,fill_char,cur_lvl,XML_MAX_INDENT_LEVEL), 
			n->value);
			
	if (n->childs && n->childs->begin)
		xmlnode_print (stream, n->childs->begin, fill_char, indent+1);
		
	if (!short_tag)
		fprintf (stream, 
			"%s</%s>\n", 
			make_indent_string (indent,fill_char,cur_lvl,XML_MAX_INDENT_LEVEL), 
			n->name);
	
	xmlnode_print (stream, n->next, fill_char, indent);
}


t_xmlattrib	*		xmlattrib_alloc (t_xmlattriblist * list, const char * const name, const char * const value)
{
	t_xmlattrib *a;
	DBGMSGNL("Allocating xmlattrib", name, XML_DBGLEVEL_ATTRIB);
	a = (t_xmlattrib*)malloc (sizeof(t_xmlattrib));
	a->name = NULL;
	a->value = NULL;
	return xmlattrib_init(a, list, name, value);
}

t_xmlattrib	*		xmlattrib_init (t_xmlattrib* attrib, t_xmlattriblist * list, const char * const name, const char * const value)
{
	if (!attrib)
		return NULL;
	DBGMSGNL("Initialising xmlattrib", name, XML_DBGLEVEL_ATTRIB);
	attrib->name = safe_strdup(name);
	attrib->value = safe_strdup(value);
	attrib->next = NULL;
	attrib->prev = NULL;
	attrib->list = list;
	return attrib;
}

void				xmlattrib_free (t_xmlattrib* a)
{
	if (!a)
		return ;
	DBGMSGL("Freeing xmlattrib", XML_DBGLEVEL_ATTRIB);
	struct_free (a, name);
	struct_free (a, value);
	if (a->next)
		a->next->prev = a->prev;
	if (a->prev)
		a->prev->next = a->next;
	if (a->list)
	{
		if (a->list->begin == a)
			a->list->begin = a->list->begin->next;
		if (a->list->end == a)
			a->list->end = a->list->end->prev;
	}
	safe_free (a);
}

t_xmlattriblist	*	xmlattriblist_alloc (t_xmlnode* parent)
{
	t_xmlattriblist *l;
	DBGMSGL("Allocating xmlattriblist", XML_DBGLEVEL_ATTRIBLIST);
	l = (t_xmlattriblist*)malloc(sizeof(t_xmlattriblist));
	xmlattriblist_init(l, parent);
	return l;	
}

t_xmlattriblist	*	xmlattriblist_init (t_xmlattriblist* l, t_xmlnode* parent)
{
	if (!l)
		return NULL;
	DBGMSGL("Initialising xmlattriblist", XML_DBGLEVEL_ATTRIBLIST);
	l->parent = parent;
	l->size = 0;
	l->begin = l->end = NULL;
	return l;
}

t_xmlattriblist	*	xmlattriblist_append (t_xmlattriblist* l, t_xmlattrib *a)
{
	DBGMSGL("Appending xmlattrib to list", XML_DBGLEVEL_ATTRIBLIST);
	if (!l)
		return l;
	if (!l->begin)
	{
		l->begin = l->end = a;
	}
	else
	{
		l->end->next = a;
		a->prev = l->end;
		l->end = a;
	}
	l->size ++;
	a->list = l;
	return l;
}

t_xmlattriblist	*	xmlattriblist_prepend (t_xmlattriblist* l, t_xmlattrib *a)
{
	if (!l)
		return l;
	DBGMSGL("Prepending xmlattrib to list", XML_DBGLEVEL_ATTRIBLIST);
	if (!l->begin)
	{
		l->begin = l->end = a;
	}
	else
	{
		l->begin->prev = a;
		a->next = l->begin;
		l->begin = a;
	}
	l->size ++;
	a->list = l;
	return l;
}

t_xmlattriblist	*	xmlattriblist_clear (t_xmlattriblist* l)
{
	t_xmlattrib *a, *aa;
	if (!l)
		return l;
	DBGMSGL("Clearing xmlattriblist", XML_DBGLEVEL_ATTRIBLIST);
	a = l->begin;
	while (a)
	{
		aa = a->next;
		xmlattrib_free (a);
		a = aa;
	}
	l->size = 0;
	return l;
}

void				xmlattriblist_free (t_xmlattriblist* l)
{
	if (!l)
		return ;
	DBGMSGL("Freeing xmlattriblist", XML_DBGLEVEL_ATTRIBLIST);
	xmlattriblist_clear (l);
	safe_free( l );
}


t_xmldocument	*	xmldoc_alloc (const char * const encoding, const char * const version, const char * const standalone,const char * const root_name, const char * const root_value)
{
	t_xmldocument * ret;
	DBGMSGL("Allocating xmldocument", XML_DBGLEVEL_DOCUMENT);
	ret = (t_xmldocument*) malloc (sizeof (t_xmldocument));
	xmldoc_init ( ret, encoding, version, standalone, root_name, root_value );
	return ret;
}

t_xmldocument	*	xmldoc_init (t_xmldocument * d, const char * const encoding, const char * const version, const char * const standalone, const char * const root_name, const char * const root_value )
{
	if (!d)
		return d;
	DBGMSGL("Initialising xmldocument", XML_DBGLEVEL_DOCUMENT);
	strncpy (d->encoding, encoding, 10);
	strncpy (d->version, version, 10);
	strncpy (d->standalone, standalone, 5);
	d->root = xmlnode_alloc (NULL, root_name, root_value);
	return d;
}

t_xmldocument	*	xmldoc_print (FILE *stream, t_xmldocument * d, char indent_char, size_t indent)
{
	if (!d ||!stream)
		return d;
	DBGMSGL("Printing xmldocument", XML_DBGLEVEL_DOCUMENT);
	fprintf (stream, "<?xml encoding=\"%s\" version=\"%s\" standalone=\"%s\" ?>\n",
					d->encoding, d->version, d->standalone );
	xmlnode_print (stream, d->root, indent_char, indent);
	return d;
}

t_xmldocument	*	xmldoc_clear (t_xmldocument * d)
{
	if (!d)
		return d;
	DBGMSGL("Clearing xmldocument", XML_DBGLEVEL_DOCUMENT);
	xmlnode_clear (d->root);
	return d;
}

void				xmldoc_free (t_xmldocument * d)
{
	if (!d)
		return ;
	DBGMSGL("Freeing xmldocument", XML_DBGLEVEL_DOCUMENT);
	xmlnode_free (d->root);
	safe_free (d);
}
