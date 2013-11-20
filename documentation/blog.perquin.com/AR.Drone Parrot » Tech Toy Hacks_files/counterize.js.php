/*
 * Start of Javascript for the Counterize plugin
 */


//a function to get an element by its id regardless of the used browser
function counterize_getElementByIdUniversal( id )
{
	var elem;
	if( document.getElementById )
	{
		elem = document.getElementById( id );
	}
	else
	{
		elem = document.all[ id ];
	}
	return elem;
}

//toggle folding of corresponding ID
function counterize_fold( sourceElemId, targetElemId )
{
	var source = counterize_getElementByIdUniversal( sourceElemId );
	var target = counterize_getElementByIdUniversal( targetElemId );

	if( target && source )
	{
		if( target.className == "collapsed" )
		{
			target.className = "expanded";
			source.innerHTML = "[&nbsp;-&nbsp;]";
		}
		else
		{
			target.className = "collapsed";
			source.innerHTML = "[&nbsp;+&nbsp;]";
		}
	}
}

//
function counterize_conf( url )
{
	if( confirm( 'Are you sure that you want to delete this entry?' ) )
	{
		self.location.href = url;
	}
}

//Add filter data to the specified field
function counterize_add_filter( data, filter_field_name )
{
	var filter_field = counterize_getElementByIdUniversal( filter_field_name );
	if( filter_field )
	{
		if( filter_field.value != '' )
		{
			var tmp = filter_field.value + '||';
			if( tmp.indexOf( data + '||' ) < 0 )
			{
				filter_field.value += '||' + data;
			}
			else
			{
				alert( 'This filter is already set!' );
			}
		}
		else
		{
			filter_field.value += data;
		}
	}
}

//Clear the fields specified in the array 'fields'
function counterize_clear_filter_form( fields )
{
	var field;
	for( var i = 0; i < fields.length; i++ )
	{
		field = counterize_getElementByIdUniversal( fields[i] );
		if( field )
		{
			field.value = "";
		}
	}
}

//
// by Nannette Thacker
// http://www.shiningstar.net
//

function counterize_check_all( field )
{
	for( i = 0; i < field.length; i++ )
	{
		field[i].checked = true ;
	}
}

function counterize_uncheck_all( field )
{
	for( i = 0; i < field.length; i++ )
	{
		field[i].checked = false ;
	}
}































/*
Behaviour v1.1 by Ben Nolan, June 2005. Based largely on the work
of Simon Willison (see comments by Simon below).

Description:

Uses css selectors to apply javascript behaviours to enable
unobtrusive javascript in html documents.

Usage:

var myrules = {
	'b.someclass' : function(element){
		element.onclick = function(){
		alert(this.innerHTML);
		}
	},
	'#someid u' : function(element){
		element.onmouseover = function(){
			this.innerHTML = "BLAH!";
		}
	}
};

Behaviour.register(myrules);

// Call Behaviour.apply() to re-apply the rules (if you
// update the dom, etc).

License:

This file is entirely BSD licensed.

More information:

http://ripcord.co.nz/behaviour/

*/

var Behaviour =
{
	list: new Array,

	register: function( sheet )
	{
		Behaviour.list.push( sheet );
	},

	start: function()
	{
		Behaviour.addLoadEvent(
			function()
			{
				Behaviour.apply();
			}
		);
	},

	apply: function()
	{
		for( h = 0; sheet = Behaviour.list[h]; h++ )
		{
			for( selector in sheet )
			{
				list = document.getElementsBySelector( selector );

				if( ! list )
				{
					continue;
				}

				for( i = 0; element = list[i]; i++ )
				{
					sheet[selector]( element );
				}
			}
		}
	},

	addLoadEvent: function( func )
	{
		var oldonload = window.onload;

		if( typeof window.onload != 'function' )
		{
			window.onload = func;
		}
		else
		{
			window.onload = function()
			{
				oldonload();
				func();
			}
		}
	}
}

Behaviour.start();

/*
The following code is Copyright (C) Simon Willison 2004.

document.getElementsBySelector(selector)
- returns an array of element objects from the current document
matching the CSS selector. Selectors can contain element names,
class names and ids and can be nested. For example:

elements = document.getElementsBySelect('div#main p a.external')

Will return an array of all 'a' elements with 'external' in their
class attribute that are contained inside 'p' elements that are
contained inside the 'div' element which has id="main"

New in version 0.4: Support for CSS2 and CSS3 attribute selectors:
See http://www.w3.org/TR/css3-selectors/#attribute-selectors

Version 0.4 - Simon Willison, March 25th 2003
-- Works in Phoenix 0.5, Mozilla 1.3, Opera 7, Internet Explorer 6, Internet Explorer 5 on Windows
-- Opera 7 fails
*
* Some improvements by GabSoftware
*/

function getAllChildren( e )
{
	// Returns all children of element. Workaround required for IE5/Windows. Ugh.
	return e.all ? e.all : e.getElementsByTagName( '*' );
}

document.getElementsBySelector = function( selector )
{
	// Attempt to fail gracefully in lesser browsers
	if( ! document.getElementsByTagName )
	{
		return new Array();
	}
	// Split selector in to tokens
	var tokens = selector.split( ' ' );
	var currentContext = new Array( document );
	for( var i = 0, tln = tokens.length; i < tln; i++ )
	{
		token = tokens[i].replace( /^\s+/, '' ).replace( /\s+$/, '' );
		if( token.indexOf( '#' ) > -1 )
		{
			// Token is an ID selector
			var bits = token.split( '#' );
			var tagName = bits[0];
			var id = bits[1];
			var element = document.getElementById( id );
			if( ! element || ( tagName && element.nodeName.toLowerCase() != tagName ) )
			{
				// tag with that ID not found, return false
				return new Array();
			}
			// Set currentContext to contain just this element
			currentContext = new Array( element );
			continue; // Skip to next token
		}
		if( token.indexOf( '.' ) > -1 )
		{
			// Token contains a class selector
			var bits = token.split( '.' );
			var tagName = bits[0];
			var className = bits[1];
			if( ! tagName )
			{
				tagName = '*';
			}
			// Get elements matching tag, filter them for class selector
			var found = new Array;
			var foundCount = 0;
			for( var h = 0, ccln = currentContext.length; h < ccln; h++ )
			{
				var elements;
				if( tagName == '*' )
				{
					elements = getAllChildren( currentContext[h] );
				}
				else
				{
					elements = currentContext[h].getElementsByTagName( tagName );
				}
				for( var j = 0, eln = elements.length; j < eln; j++ )
				{
					found[foundCount++] = elements[j];
				}
			}
			currentContext = new Array;
			var currentContextIndex = 0;
			for( var k = 0, fln = found.length; k < fln; k++ )
			{
				if( found[k].className && found[k].className.match( new RegExp( '(\\s|^)' + className + '(\\s|$)' ) ) )
				{
					currentContext[currentContextIndex++] = found[k];
				}
			}
			continue; // Skip to next token
		}
		// Code to deal with attribute selectors
		if( token.match( /^(\w*)\[(\w+)([=~\|\^\$\*]?)=?"?([^\]"]*)"?\]$/ ) )
		{
			var tagName = RegExp.$1;
			var attrName = RegExp.$2;
			var attrOperator = RegExp.$3;
			var attrValue = RegExp.$4;
			if( ! tagName )
			{
				tagName = '*';
			}
			// Grab all of the tagName elements within current context
			var found = new Array;
			var foundCount = 0;
			for( var h = 0, ccln = currentContext.length; h < ccln; h++ )
			{
				var elements;
				if( tagName == '*' )
				{
					elements = getAllChildren(currentContext[h] );
				}
				else
				{
					elements = currentContext[h].getElementsByTagName( tagName );
				}
				for( var j = 0, eln = elements.length; j < eln; j++ )
				{
					found[foundCount++] = elements[j];
				}
			}
			currentContext = new Array;
			var currentContextIndex = 0;
			var checkFunction; // This function will be used to filter the elements
			switch( attrOperator )
			{
				case '=': // Equality
					checkFunction = function( e ) { return ( e.getAttribute( attrName ) == attrValue ); };
					break;
				case '~': // Match one of space seperated words
					checkFunction = function( e ) { return ( e.getAttribute( attrName ).match( new RegExp( '(\\s|^)' + attrValue + '(\\s|$)' ) ) ); };
					break;
				case '|': // Match start with value followed by optional hyphen
					checkFunction = function( e ) { return ( e.getAttribute( attrName ).match( new RegExp( '^' + attrValue + '-?' ) ) ); };
					break;
				case '^': // Match starts with value
					checkFunction = function( e ) { return ( e.getAttribute( attrName ).indexOf( attrValue ) == 0 ); };
					break;
				case '$': // Match ends with value - fails with "Warning" in Opera 7
					checkFunction = function( e ) { return ( e.getAttribute( attrName ).lastIndexOf( attrValue ) == e.getAttribute( attrName ).length - attrValue.length ); };
					break;
				case '*': // Match ends with value
					checkFunction = function( e ) { return ( e.getAttribute( attrName ).indexOf( attrValue ) > -1 ); };
					break;
				default :
				// Just test for existence of attribute
					checkFunction = function( e ) { return e.getAttribute( attrName ); };
			}
			currentContext = new Array;
			var currentContextIndex = 0;
			for( var k = 0, fln = found.length; k < fln; k++ )
			{
				if (checkFunction(found[k]))
				{
					currentContext[currentContextIndex++] = found[k];
				}
			}
			// alert('Attribute Selector: '+tagName+' '+attrName+' '+attrOperator+' '+attrValue);
			continue; // Skip to next token
		}

		if( ! currentContext[0] )
		{
			return;
		}

		// If we get here, token is JUST an element (not a class or ID selector)
		tagName = token;
		var found = new Array;
		var foundCount = 0;
		for( var h = 0, ccln = currentContext.length; h < ccln; h++ )
		{
			var elements = currentContext[h].getElementsByTagName( tagName );
			for( var j = 0, eln = elements.length; j < eln; j++ )
			{
				found[foundCount++] = elements[j];
			}
		}
		currentContext = found;
	}
	return currentContext;
}

/* That revolting regular expression explained
/^(\w+)\[(\w+)([=~\|\^\$\*]?)=?"?([^\]"]*)"?\]$/
\---/  \---/\-------------/    \-------/
|      |         |               |
|      |         |           The value
|      |    ~,|,^,$,* or =
|   Attribute
Tag
*/

var myrules =
{
	'a': function( el )
	{
		if( document.domain )
		{
			if( el.onclick != null || el.href.substring( 0, 1 ) == '/' || el.href.substring( 0, 1 ) == '#' || el.href.substring( 0, 11 ) == 'javascript:' || el.href.indexOf( document.domain ) > -1 )
			{
				return;
			}
			else
			{
				el.onclick = function()
				{
					var request = false;
					try
					{
						request = new XMLHttpRequest();
					}
					catch( trymicrosoft )
					{
						try
						{
							request = new ActiveXObject( 'Msxml2.XMLHTTP' );
						}
						catch( othermicrosoft )
						{
							try
							{
								request = new ActiveXObject( 'Microsoft.XMLHTTP' );
							}
							catch( failed )
							{
								request = false;
							}
						}
					}

					if( ! request )
					{
						alert( 'Error initializing XMLHttpRequest!' );
						return;
					}


					var url = '/wp-content/plugins/counterize/counterize.php?external=1&href=' + escape( this.href ) + '&from=' + escape( window.location.href ) + '&time=1384966086';
					request.abort();
					request.open( 'GET', url, false );
					request.onreadystatechange =
						function()
						{
							if( request.readyState == 4 )
							{
								if( request.status == 200 )
								{
									var response = request.responseText;
									//alert( 'response: ' + response );
								}
								else
								{
									//alert( request.status );
								}
							}
						};
					request.send( null );
				}
			}
		}
	}
};
Behaviour.register( myrules );






//
// End of Counterize Javascript
//