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

function insert_household_basic()
{
	$household_info = array(
		array( 'company_id' => 1,
		       'is_group' => 1, 
			   'subgroup_cnt' => 50,
			   'have_website' => 1,
			   'website_addr' => 'www.ainong.cn',
			   'service_type' => 1,
			 ),
		array( 'company_id' => 2,
		       'is_group' => 1, 
			   'subgroup_cnt' => 10,
			   'have_website' => 1,
			   'website_addr' => 'www.taiweifeng.com',
			   'service_type' => 1,
			 ),
	);

	foreach ($household_info as $row) {
		$id = db_insert('youwo_household_basic')
		   ->fields(array(
		     'company_id' => $row['company_id'],
		     'is_group' => $row['is_group'],
		     'subgroup_cnt' => $row['subgroup_cnt'],
		     'have_website' => $row['have_website'],
		     'website_addr' => $row['website_addr'],
		     'service_type' => $row['service_type'],
			 ))
		   ->execute();
	}
	return;
}

function remove_data() {
   $num_deleted = db_delete('youwo_household_basic')
       ->execute();

   $num_deleted = db_delete('youwo_company')
       ->execute();
}
	


function youwo_install_data() {
	remove_data();
	insert_youwo_company();
	insert_household_basic();
	return "";
}
