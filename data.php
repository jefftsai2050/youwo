<?php

function insert_youwo_company() {
	$company_info = array(
		array('company_name' => '北京市爱侬家政服务有限责任公司',
		      'company_brief_name' => '爱侬家政',
			  'tid' => 0),
		array('company_name' => '北京市泰维峰家政公司',
		      'company_brief_name' => '泰维峰家政',
			  'tid' => 0),
	    );

	foreach ($company_info as $row) {
		$id = db_insert('youwo_company')
		   ->fields(array(
		     'company_name' => $row['company_name'],
			 'company_brief_name' => $row['company_brief_name'],
			 'tid' => $row['tid'],
			 ))
		   ->execute();
    }

	return;
}

function youwo_install_data() {
	insert_youwo_company();
	return "";
}
