/* Copyright (c) 2006 Brandon Aaron (http://brandonaaron.net)
 * Dual licensed under the MIT (http://www.opensource.org/licenses/mit-license.php)
 * and GPL (http://www.opensource.org/licenses/gpl-license.php) licenses.
 *
 * $LastChangedDate: 2007-06-19 20:25:28 -0500 (Tue, 19 Jun 2007) $
 * $Rev: 2111 $
 *
 * Version 2.1
 */
(function($){$.fn.bgIframe=$.fn.bgiframe=function(s){if($.browser.msie&&parseInt($.browser.version)<=6){s=$.extend({top:'auto',left:'auto',width:'auto',height:'auto',opacity:true,src:'javascript:false;'},s||{});var prop=function(n){return n&&n.constructor==Number?n+'px':n;},html='<iframe class="bgiframe"frameborder="0"tabindex="-1"src="'+s.src+'"'+'style="display:block;position:absolute;z-index:-1;'+(s.opacity!==false?'filter:Alpha(Opacity=\'0\');':'')+'top:'+(s.top=='auto'?'expression(((parseInt(this.parentNode.currentStyle.borderTopWidth)||0)*-1)+\'px\')':prop(s.top))+';'+'left:'+(s.left=='auto'?'expression(((parseInt(this.parentNode.currentStyle.borderLeftWidth)||0)*-1)+\'px\')':prop(s.left))+';'+'width:'+(s.width=='auto'?'expression(this.parentNode.offsetWidth+\'px\')':prop(s.width))+';'+'height:'+(s.height=='auto'?'expression(this.parentNode.offsetHeight+\'px\')':prop(s.height))+';'+'"/>';return this.each(function(){if($('> iframe.bgiframe',this).length==0)this.insertBefore(document.createElement(html),this.firstChild);});}return this;};if(!$.browser.version)$.browser.version=navigator.userAgent.toLowerCase().match(/.+(?:rv|it|ra|ie)[\/: ]([\d.]+)/)[1];})(jQuery);

/*************************************************
**  jQuery Masonry version 1.3.2
**  Copyright David DeSandro, licensed MIT
**  http://desandro.com/resources/jquery-masonry
**************************************************/
(function(e){var n=e.event,o;n.special.smartresize={setup:function(){e(this).bind("resize",n.special.smartresize.handler)},teardown:function(){e(this).unbind("resize",n.special.smartresize.handler)},handler:function(j,l){var g=this,d=arguments;j.type="smartresize";o&&clearTimeout(o);o=setTimeout(function(){jQuery.event.handle.apply(g,d)},l==="execAsap"?0:100)}};e.fn.smartresize=function(j){return j?this.bind("smartresize",j):this.trigger("smartresize",["execAsap"])};e.fn.masonry=function(j,l){var g={getBricks:function(d,b,a){var c=a.itemSelector===undefined;b.$bricks=a.appendedContent===undefined?c?d.children():d.find(a.itemSelector):c?a.appendedContent:a.appendedContent.filter(a.itemSelector)},placeBrick:function(d,b,a,c,h){b=Math.min.apply(Math,a);for(var i=b+d.outerHeight(true),f=a.length,k=f,m=c.colCount+1-f;f--;)if(a[f]==b)k=f;d.applyStyle({left:c.colW*k+c.posLeft,top:b},e.extend(true,{},h.animationOptions));for(f=0;f<m;f++)c.colY[k+f]=i},setup:function(d,b,a){g.getBricks(d,a,b);if(a.masoned)a.previousData=d.data("masonry");a.colW=b.columnWidth===undefined?a.masoned?a.previousData.colW:a.$bricks.outerWidth(true):b.columnWidth;a.colCount=Math.floor(d.width()/a.colW);a.colCount=Math.max(a.colCount,1)},arrange:function(d,b,a){var c;if(!a.masoned||b.appendedContent!==undefined)a.$bricks.css("position","absolute");if(a.masoned){a.posTop=a.previousData.posTop;a.posLeft=a.previousData.posLeft}else{d.css("position","relative");var h=e(document.createElement("div"));d.prepend(h);a.posTop=Math.round(h.position().top);a.posLeft=Math.round(h.position().left);h.remove()}if(a.masoned&&b.appendedContent!==undefined){a.colY=a.previousData.colY;for(c=a.previousData.colCount;c<a.colCount;c++)a.colY[c]=a.posTop}else{a.colY=[];for(c=a.colCount;c--;)a.colY.push(a.posTop)}e.fn.applyStyle=a.masoned&&b.animate?e.fn.animate:e.fn.css;b.singleMode?a.$bricks.each(function(){var i=e(this);g.placeBrick(i,a.colCount,a.colY,a,b)}):a.$bricks.each(function(){var i=e(this),f=Math.ceil(i.outerWidth(true)/a.colW);f=Math.min(f,a.colCount);if(f===1)g.placeBrick(i,a.colCount,a.colY,a,b);else{var k=a.colCount+1-f,m=[];for(c=0;c<k;c++){var p=a.colY.slice(c,c+f);m[c]=Math.max.apply(Math,p)}g.placeBrick(i,k,m,a,b)}});a.wallH=Math.max.apply(Math,a.colY);d.applyStyle({height:a.wallH-a.posTop},e.extend(true,[],b.animationOptions));a.masoned||setTimeout(function(){d.addClass("masoned")},1);l.call(a.$bricks);d.data("masonry",a)},resize:function(d,b,a){a.masoned=!!d.data("masonry");var c=d.data("masonry").colCount;g.setup(d,b,a);a.colCount!=c&&g.arrange(d,b,a)}};return this.each(function(){var d=e(this),b={};b.masoned=!!d.data("masonry");var a=b.masoned?d.data("masonry").options:{},c=e.extend({},e.fn.masonry.defaults,a,j),h=a.resizeable;b.options=c.saveOptions?c:a;l=l||function(){};g.getBricks(d,b,c);if(!b.$bricks.length)return this;g.setup(d,c,b);g.arrange(d,c,b);!h&&c.resizeable&&e(window).bind("smartresize.masonry",function(){g.resize(d,c,b)});h&&!c.resizeable&&e(window).unbind("smartresize.masonry")})};e.fn.masonry.defaults={singleMode:false,columnWidth:undefined,itemSelector:undefined,appendedContent:undefined,saveOptions:true,resizeable:true,animate:false,animationOptions:{}}})(jQuery);

/**
 * suffusion.js - Contains all custom JavaScript functions required by Suffusion
 */

$j = jQuery.noConflict();

$j.fn.extend({
	highlight: function(search, insensitive, hls_class){
		var regex = new RegExp("(<[^>]*>)|(\\b"+ search.replace(/([-.*+?^${}()|[\]\/\\])/g,"\\$1") +")", insensitive ? "ig" : "g");
		return this.html(this.html().replace(regex, function(a, b, c){
			return (a.charAt(0) == "<") ? a : "<span class=\""+ hls_class +"\">" + c + "</span>";
		}));
	}
});

$j.fn.removeHighlight = function() {
	return this.find("span.search-highlight").each(function() {
		with (this.parentNode) {
			replaceChild(this.firstChild, this);
			normalize();
		}
	}).end();
};

$j(document).ready(function() {
    /* Magazine JS, for the headline section */
	$j('div.suf-mag-headlines div.suf-mag-headline-photo').hide();
	$j('div.suf-mag-headline-photo-first').show();
	$j('div.suf-mag-headline-block ul.mag-headlines li.suf-mag-headline-first a').addClass('tab-current');

	$j('div.suf-mag-headline-block ul.mag-headlines li a').hover(function(){
		var thisClass = this.className.substring(17, this.className.indexOf(" "));
		$j('div.suf-mag-headlines div.suf-mag-headline-photo').hide();
		$j('div.suf-mag-headline-block ul.mag-headlines li a').removeClass('tab-current');
        $j('div.suf-mag-headline-photo-' + thisClass).show();
		$j(this).addClass('tab-current');
	}   );
    /* End Magazine JS */

    /* Tabbed Widgets */
    $j('div.tab-box div.sidebar-tab-content').hide();
    $j('div.sbtab-content-first').show();
    $j('div.tab-box ul.sidebar-tabs li.sbtab-first a').addClass('tab-current');

    $j('div.tab-box ul.sidebar-tabs li a').click(function(){
        var thisClass = this.className.substring(6, this.className.indexOf(" "));
        $j('div.tab-box div.sidebar-tab-content').hide();
        $j('div.tab-box div.sbtab-content-' + thisClass).show();
        $j('div.tab-box ul.sidebar-tabs li a').removeClass('tab-current');
        $j(this).addClass('tab-current');
    });
    /* End Tabbed Widgets */

    /* JQuery-based fix for widgets that do not print "after_title" */
    $j('.dbx-handle').each(function() {
        $j(this).prependTo(this.parentNode.parentNode);
    });
    /* End JQuery-based fix */

	/* JQuery for tabbed sidebars */
	$j('.sidebar-tab .sidebar-tab-content').each(function() {
		var parentId = this.parentNode.id;
		var parentClass = this.parentNode.className;
		parentClass = parentClass.substring(12);
		$j(this).addClass('sbtab-content-' + parentId);
		$j(this).addClass(parentClass);
	    $j(this).appendTo(this.parentNode.parentNode.parentNode);
	});

	$j('.tabbed-sidebar ul.sidebar-tabs a').each(function() {
		var parentId = this.parentNode.id;
		$j(this).addClass(parentId);
	});

	$j('#wsidebar-top.tabbed-sidebar ul.sidebar-tabs li:first').addClass('sbtab-first');
	$j('#wsidebar-top.tabbed-sidebar div.sidebar-tab-content:first').addClass('sbtab-content-first');
	$j('#sidebar.tabbed-sidebar ul.sidebar-tabs li:first').addClass('sbtab-first');
	$j('#sidebar.tabbed-sidebar div.sidebar-tab-content:first').addClass('sbtab-content-first');
	$j('#sidebar-b.tabbed-sidebar ul.sidebar-tabs li:first').addClass('sbtab-first');
	$j('#sidebar-b.tabbed-sidebar div.sidebar-tab-content:first').addClass('sbtab-content-first');
	$j('#sidebar-2.tabbed-sidebar ul.sidebar-tabs li:first').addClass('sbtab-first');
	$j('#sidebar-2.tabbed-sidebar div.sidebar-tab-content:first').addClass('sbtab-content-first');
	$j('#sidebar-2-b.tabbed-sidebar ul.sidebar-tabs li:first').addClass('sbtab-first');
	$j('#sidebar-2-b.tabbed-sidebar div.sidebar-tab-content:first').addClass('sbtab-content-first');
	$j('#wsidebar-bottom.tabbed-sidebar ul.sidebar-tabs li:first').addClass('sbtab-first');
	$j('#wsidebar-bottom.tabbed-sidebar div.sidebar-tab-content:first').addClass('sbtab-content-first');

	$j('div.tabbed-sidebar div.sidebar-tab-content').hide();
	$j('div.sbtab-content-first').show();
	$j('div.tabbed-sidebar ul.sidebar-tabs li.sbtab-first a').addClass('tab-current');

	$j('div.tabbed-sidebar ul.sidebar-tabs li a').click(function(){
		$j(this).removeClass('tab-current');
	    var thisClass = this.className.substring(12, this.className.length);
		var parentId = this.parentNode.parentNode.parentNode.id;
	    $j('#' + parentId + '.tabbed-sidebar div.sidebar-tab-content').hide();
	    $j('#' + parentId + '.tabbed-sidebar div.sbtab-content-' + thisClass).show();
	    $j('#' + parentId + '.tabbed-sidebar ul.sidebar-tabs li a').removeClass('tab-current');
	    $j(this).addClass('tab-current');
	});
	/* End tabbed sidebars */

	$j("ul.sf-menu > li > ul").each(function() {
		var parentClass = this.parentNode.className;
		var a_tag_h = this.parentNode.childNodes[0];
		var a_tag = $j(a_tag_h);
		a_tag.prepend("<!--[if !IE]>--><span class='down-ptr'>&nbsp;</span><!--<![endif]-->");
		a_tag.append("<!--[if lt IE 7]>&nbsp;&nbsp;&darr;<![endif]--> <!--[if (!IE)|(gte IE 7)]><span class='down-ptr'>&nbsp;</span><![endif]-->");
		a_tag.addClass('with-arrow');
	});

	$j("ul.sf-menu ul ul").each(function() {
		var parentClass = this.parentNode.className;
		var a_tag_h = this.parentNode.childNodes[0];
		var a_tag = $j(a_tag_h);
		a_tag.prepend("<!--[if !IE]>--><span class='float-ptr'>&nbsp;</span><!--<![endif]-->");
		a_tag.append("<!--[if lt IE 7]>&nbsp;&nbsp;&rarr;<![endif]--> <!--[if (!IE)|(gte IE 7)]><span class='float-ptr'>&nbsp;</span><![endif]-->");
		a_tag.addClass('drop');
	});

    $j("#nav ul.sf-menu li").hover(
            function(){
                if (suf_nav_effect == 'fade') {
                    $j(this).find('ul:first').show(suf_nav_delay).fadeIn().css({visibility: "visible"}); /*  */
                }
                else {
                    $j(this).find('ul:first').show(suf_nav_delay).css({visibility: "visible"}); /*  */
                }
            },
            function(){
                $j(this).find('ul:first').css({visibility: "hidden"}); /*  */
            });//.bgIframe({opacity: 'show'});

	$j("#nav-top ul.sf-menu li").hover(
	        function(){
	            if (suf_navt_effect == 'fade') {
	                $j(this).find('ul:first').show(suf_navt_delay).fadeIn().css({visibility: "visible"}); /*  */
	            }
	            else {
	                $j(this).find('ul:first').show(suf_navt_delay).css({visibility: "visible"}); /*  */
	            }
	        },
	        function(){
	            $j(this).find('ul:first').css({visibility: "hidden"}); /*  */
	        }).bgIframe({opacity: 'show'});

	// Expand Panel
	$j("#top-bar-right-spanel-tab .open").click(function(){
		$j("#top-bar-right-spanel").slideDown("medium");
	});
	// Collapse Panel
	$j("#top-bar-right-spanel-tab .close").click(function(){
		$j("#top-bar-right-spanel").slideUp("medium");
	});
	// Switch buttons from "Log In | Register" to "Close Panel" on click
	$j("#top-bar-right-spanel-tab .toggle a").click(function () {
		$j("#top-bar-right-spanel-tab .toggle a").toggle();
	});

	$j('#search-info input[type=checkbox].search-hl').change(function(event) {
		var search = $j('#search-term').val();
		var hls_class = 'search-highlight';
		if ($j(this).attr('checked')) {
			// Highlight search results
			$j('#content .entry').each(function() {
				var quotes = search.match(/"([^"]+)"/g);
				var len = 0;
				var matches = new Array();
				if (quotes != null) {
					len = quotes.length;
					for (i=0; i<len; i++) {
						quotes[i] = quotes[i].replace(/"/g, '');
						matches[matches.length] = quotes[i];
						search = search.replace(quotes[i], ' ');
					}
				}

				var no_quotes = search.match(/\b[\S]+\b/g);
				if (no_quotes != null) {
					len = no_quotes.length;
					for (i=0; i<len; i++) {
						matches[matches.length] = no_quotes[i];
					}
				}

				len = matches.length;
				for (i=0; i<len; i++) {
					var search_string = matches[i];
					$j(this).highlight(search_string);
					$j(this).highlight(search_string, true, hls_class);
				}
			});
		}
		else {
			//Un-highlight search results
			$j('#content .entry').each(function() {
				$j(this).removeHighlight();
			});
		}
	});

	if (typeof suf_jq_masonry_enabled != 'undefined' && suf_jq_masonry_enabled == 'enabled') {
		//$j("#top-bar-right-spanel").masonry();
		//$j("#widgets-above-header").masonry();
		$j("#horizontal-outer-widgets-1").masonry();
		$j("#horizontal-outer-widgets-2").masonry();
		$j("#ad-hoc-1").masonry();
		$j("#ad-hoc-2").masonry();
		$j("#ad-hoc-3").masonry();
		$j("#ad-hoc-4").masonry();
		$j("#ad-hoc-5").masonry();
	}

	// If the HTML height attribute is present for images, the max-width setting really hammers the image proportions.
	$j(".entry img").removeAttr('height');

	// Fixes for NGG on IE8. This should ideally be on NGG's plugin itself, but given their support or lack thereof :-( ...
	$j(".ngg-gallery-thumbnail img").removeAttr('height');
	$j(".ngg-gallery-thumbnail img").removeAttr('width');

	$j(".widget_calendar tbody td a").each(function() {
		var parent = this.parentNode;
		//var a_tag_h = this.parentNode.childNodes[0];
		var td_tag = $j(parent);
		td_tag.addClass('with-posts');
	});
});

