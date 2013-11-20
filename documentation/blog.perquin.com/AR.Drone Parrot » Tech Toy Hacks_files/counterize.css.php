/*
 * Counterize CSS
 */
.collapsed {
	display: none;
}

.expanded {
	display: table-row;
}

#counterize_history_navigationbar *, #counterize_history_navigationbar2 * {
	display: inline-block;
}

#counterize_history_navigationbar li, #counterize_history_navigationbar2 li {
	margin-left: 5px;
	margin-right: 5px;
}


#counterizehistorytable {
	margin-bottom: 30px;
}

#counterizehistorytable tr.alternate, #counterizehitcountertable tr.alternate {
	background-color: #e8e8e8;
}

#counterizehistorytable tbody {
	border: solid #999 1px;
}

#counterizehistorytable thead {
	display: table-header-group;
	border: solid #999 1px;
}

#counterizehistorytable th {
	height: 30px;
	background-color: #ddd;
}

#counterizehistorytable thead tr th, #counterizehistorytable tfoot tr th, #counterizehistorytable tbody tr.repeat th {
	padding: 5px;
	text-align: center;
}

#counterizehistorytable tfoot {
	display: table-footer-group;
	border: solid #999 1px;
}

#counterizehistorytable tbody tr.repeat {
	border: solid #999 1px;
}

#counterizehistorytable th + th, #counterizehistorytable td + td {
	border-left: dashed #d0d0d0 1px;
	padding: 5px;
}

#counterizehistorytable td {
	vertical-align: middle;
}

.counterize_history_killbutton {
	color: red;
	font-size: 18px;
	font-weight: bold;
}

.counterize_subtable {
	border: solid #ccc 1px;
}

.counterize_subtable .alternate {
	background-color: #eee;
}

.counterize_caption_help:hover {
	cursor: help;
}

.counterize_chart_bar {
	box-shadow:2px 2px 2px #333333;
}

.counterize_chart_bar_horizontal {
	vertical-align:middle;
	margin-bottom: 1px;
	height: 20px;
	display: inline-block;
}

/* horizontal gradients */
.counterize_chart_bar_horizontal.blue {
	background: #003399; /* Old browsers */
	background: -moz-linear-gradient(left, #003399 0%, #006699 100%); /* FF3.6+ */
	background: -webkit-gradient(linear, left top, right top, color-stop(0%,#003399), color-stop(100%,#006699)); /* Chrome,Safari4+ */
	background: -webkit-linear-gradient(left, #003399 0%,#006699 100%); /* Chrome10+,Safari5.1+ */
	background: -o-linear-gradient(left, #003399 0%,#006699 100%); /* Opera 11.10+ */
	background: -ms-linear-gradient(left, #003399 0%,#006699 100%); /* IE10+ */
	background: linear-gradient(left, #003399 0%,#006699 100%); /* W3C */
}
.counterize_chart_bar_horizontal.red {
	background: #cc0000; /* Old browsers */
	background: -moz-linear-gradient(left, #cc0000 0%, #ff0000 100%); /* FF3.6+ */
	background: -webkit-gradient(linear, left top, right top, color-stop(0%,#cc0000), color-stop(100%,#ff0000)); /* Chrome,Safari4+ */
	background: -webkit-linear-gradient(left, #cc0000 0%,#ff0000 100%); /* Chrome10+,Safari5.1+ */
	background: -o-linear-gradient(left, #cc0000 0%,#ff0000 100%); /* Opera 11.10+ */
	background: -ms-linear-gradient(left, #cc0000 0%,#ff0000 100%); /* IE10+ */
	background: linear-gradient(left, #cc0000 0%,#ff0000 100%); /* W3C */
}
.counterize_chart_bar_horizontal.green {
	background: #009900; /* Old browsers */
	background: -moz-linear-gradient(left, #009900 0%, #00dc00 100%); /* FF3.6+ */
	background: -webkit-gradient(linear, left top, right top, color-stop(0%,#009900), color-stop(100%,#00dc00)); /* Chrome,Safari4+ */
	background: -webkit-linear-gradient(left, #009900 0%,#00dc00 100%); /* Chrome10+,Safari5.1+ */
	background: -o-linear-gradient(left, #009900 0%,#00dc00 100%); /* Opera 11.10+ */
	background: -ms-linear-gradient(left, #009900 0%,#00dc00 100%); /* IE10+ */
	background: linear-gradient(left, #009900 0%,#00dc00 100%); /* W3C */
}
.counterize_chart_bar_horizontal.yellow {
	background: #ccc00c; /* Old browsers */
	background: -moz-linear-gradient(left, #ccc00c 0%, #ffff00 100%); /* FF3.6+ */
	background: -webkit-gradient(linear, left top, right top, color-stop(0%,#ccc00c), color-stop(100%,#ffff00)); /* Chrome,Safari4+ */
	background: -webkit-linear-gradient(left, #ccc00c 0%,#ffff00 100%); /* Chrome10+,Safari5.1+ */
	background: -o-linear-gradient(left, #ccc00c 0%,#ffff00 100%); /* Opera 11.10+ */
	background: -ms-linear-gradient(left, #ccc00c 0%,#ffff00 100%); /* IE10+ */
	background: linear-gradient(left, #ccc00c 0%,#ffff00 100%); /* W3C */
}

.counterize_chart_bar_vertical {
	width: 20px;
	vertical-align: bottom;
	margin-right: 1px;
	display: inline-block;
}

/* vertical gradients */
.counterize_chart_bar_vertical.blue {
	background: #006699; /* Old browsers */
	background: -moz-linear-gradient(top, #006699 0%, #003399 100%); /* FF3.6+ */
	background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,#006699), color-stop(100%,#003399)); /* Chrome,Safari4+ */
	background: -webkit-linear-gradient(top, #006699 0%,#003399 100%); /* Chrome10+,Safari5.1+ */
	background: -o-linear-gradient(top, #006699 0%,#003399 100%); /* Opera 11.10+ */
	background: -ms-linear-gradient(top, #006699 0%,#003399 100%); /* IE10+ */
	background: linear-gradient(top, #006699 0%,#003399 100%); /* W3C */
}
.counterize_chart_bar_vertical.red {
	background: #ff0000; /* Old browsers */
	background: -moz-linear-gradient(top, #ff0000 0%, #cc0000 100%); /* FF3.6+ */
	background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,#ff0000), color-stop(100%,#cc0000)); /* Chrome,Safari4+ */
	background: -webkit-linear-gradient(top, #ff0000 0%,#cc0000 100%); /* Chrome10+,Safari5.1+ */
	background: -o-linear-gradient(top, #ff0000 0%,#cc0000 100%); /* Opera 11.10+ */
	background: -ms-linear-gradient(top, #ff0000 0%,#cc0000 100%); /* IE10+ */
	background: linear-gradient(top, #ff0000 0%,#cc0000 100%); /* W3C */
}
.counterize_chart_bar_vertical.green {
	background: #00dc00; /* Old browsers */
	background: -moz-linear-gradient(top, #00dc00 0%, #009900 100%); /* FF3.6+ */
	background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,#00dc00), color-stop(100%,#009900)); /* Chrome,Safari4+ */
	background: -webkit-linear-gradient(top, #00dc00 0%,#009900 100%); /* Chrome10+,Safari5.1+ */
	background: -o-linear-gradient(top, #00dc00 0%,#009900 100%); /* Opera 11.10+ */
	background: -ms-linear-gradient(top, #00dc00 0%,#009900 100%); /* IE10+ */
	background: linear-gradient(top, #00dc00 0%,#009900 100%); /* W3C */
}
.counterize_chart_bar_vertical.yellow {
	background: #ffff00; /* Old browsers */
	background: -moz-linear-gradient(top, #ffff00 0%, #cccc00 100%); /* FF3.6+ */
	background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,#ffff00), color-stop(100%,#cccc00)); /* Chrome,Safari4+ */
	background: -webkit-linear-gradient(top, #ffff00 0%,#cccc00 100%); /* Chrome10+,Safari5.1+ */
	background: -o-linear-gradient(top, #ffff00 0%,#cccc00 100%); /* Opera 11.10+ */
	background: -ms-linear-gradient(top, #ffff00 0%,#cccc00 100%); /* IE10+ */
	background: linear-gradient(top, #ffff00 0%,#cccc00 100%); /* W3C */
}

.counterize_chart_bar_vertical_inverted {
	vertical-align: top;
	-moz-transform: scaleY(-1);
	-o-transform: scaleY(-1);
	-webkit-transform: scaleY(-1);
	-ms-filter: 'FlipV';
	transform: scaleY(-1);
	filter: FlipV;
}

.counterizefooter {
	margin-top: 30px;
}