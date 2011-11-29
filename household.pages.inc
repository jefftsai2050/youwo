<?php
function household_info() {

	//Create a list of headers for your Html table (see Drupal 7 docs for theme_table here: http://api.drupal.org/api/drupal/includes--theme.inc/function/theme_table/7
	$header = array(
		array('data' => '公司编号', 'field' => 'company_id', 'sort' => 'asc'),
		array('data' => 'google检索数', 'field' => 'google_count'),
		array('data' => '有啊', 'field' => 'have_youa'),
		array('data' => '58同城', 'field' => 'have_tongcheng'),
		array('data' => '口碑', 'field' => 'have_koubei'),
		);

	//Create the Sql query. This uses various parts of Drupal 7's new DBTNG database abstraction layer. See these links for details:
	//Dynamic queries: http://drupal.org/node/310075
	//Extenders: http://drupal.org/node/508796
	//Table sort: http://drupal.org/node/310075#table_sort
	
	$query = db_select('youwo_household_info', 'h')
//				->condition('status', 1)	//Only published nodes, change condition as it suits you
				->extend('PagerDefault') 	//Pager Extender
					->limit(10)				//10 results per page
				->extend('TableSort')		//Sorting Extender
					->orderByHeader($header)//Field to sort on is picked from $header
				->fields ('h', array (
					'company_id',
					'google_count',
					'have_youa',
					'have_tongcheng',
					'have_koubei',
				));
	
	$results = $query
				->execute();
	
	$rows = array();	
	foreach ($results as $node) {
		$rows[] = array(
					'data' => array(
						  $node->company_id,
						  $node->google_count,
						  $node->have_youa,
						  $node->have_tongcheng,
						  $node->have_koubei
					)
		);
	}
	
	//Theme the html table: http://api.drupal.org/api/drupal/includes--theme.inc/function/theme_table/7
	$html = theme('table', 
				array(
					'header' => $header,
					'rows'=>$rows,
					'caption' => '家政公司信息',	//Optional Caption for the table
					'sticky' => TRUE,						//Optional to indicate whether the table headers should be sticky
					'empty' => 'No nodes created...',		//Optional empty text for the table if resultset is empty
				)
			);
	
	//Append pager: http://api.drupal.org/api/drupal/includes--pager.inc/function/theme_pager
	$html .= theme('pager',
				array(
					'tags' => array()
				)
			);
			
	return ($html);
}
