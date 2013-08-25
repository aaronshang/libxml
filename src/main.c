#include "xml.h"
#include "stdio.h"

int main (int argc, char *argv [])
{
	t_xmlnode * html, * body, * head;
	t_xmlnode * div, * title;
	
	FILE * f;
	(void) argc;
	(void) argv;
	puts("XML Lib Test Unit");
	html = xmlnode_alloc (NULL, "html", "");
	head = xmlnode_alloc (NULL, "head", "");
	body = xmlnode_alloc (NULL, "body", "");
	
	div = xmlnode_alloc (NULL, "div", "");
	title = xmlnode_alloc (NULL, "title", "Test XML");
	
	xmlattriblist_append (div->attribs, xmlattrib_alloc(NULL, "id", "main_div"));
	xmlattriblist_append (div->attribs, xmlattrib_alloc(NULL, "class", "main_div"));
	
	xmllist_append (html->childs, head);
	xmllist_append (html->childs, body);
		
	xmllist_append (head->childs, title);
	xmllist_append (body->childs, div);
	
	f = fopen ("/home/darkboss/tmp/test.xml", "w" );
	xmlnode_print (f, html, '\t', 0);
	fclose (f);
	
	xmlnode_free (html);
	
	return 0;
}
