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
		array('company_name' => '北京嘉乐会家政服务有限公司',
		      'company_brief_name' => '嘉乐会家政',
			  'tid' => 0),
	    );
		array('company_name' => '北京华夏中青家政服务有限公司',
		      'company_brief_name' => '华夏中青家政',
			  'tid' => 0),
	    );
		array('company_name' => '北京市春光家美家政服务公司',
		      'company_brief_name' => '春光家美家政',
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
		array( 'company_id' => 3,
		       'is_group' => 1, 
			   'subgroup_cnt' => 10,
			   'have_website' => 1,
			   'website_addr' => 'www.coleclub.com',
			   'service_type' => 1,
			 ),
		array( 'company_id' => 4,
		       'is_group' => 1, 
			   'subgroup_cnt' => 3,
			   'have_website' => 1,
			   'website_addr' => 'hxzqjz.com',
			   'service_type' => 1,
			 ),
		array( 'company_id' => 5,
		       'is_group' => 1, 
			   'subgroup_cnt' => 9,
			   'have_website' => 1,
			   'website_addr' => 'www.bjcgjmjz.com',
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

function insert_household_info()
{
	$household_info = array(
		array( 'company_id' => 1,
		       'google_cnt' => 1030000, 
			   'baidu_cnt' => 296000,
			   'good_comment_cnt' => 0,
			   'bad_comment_cnt' => 0,
			   'bad_comment_links' => 'http://beijing.koubei.com/store/detail--storeId-12819599df774426ae273bef5c48c967#dplist-container;http://beijing.koubei.com/store/detail--storeId-90f84beabdb44504b58f451b0717295f',
			 ),
		array( 'company_id' => 2,
		       'google_cnt' => 11700000, 
			   'baidu_cnt' => 36600,
			   'good_comment_cnt' => 0,
			   'bad_comment_cnt' => 0,
			 ),
		array( 'company_id' => 3,
		       'google_cnt' => 862000, 
			   'baidu_cnt' => 53100,
			   'good_comment_cnt' => 0,
			   'bad_comment_cnt' => 0,
			 ),
		array( 'company_id' => 4,
		       'google_cnt' => 2740000, 
			   'baidu_cnt' => 265000,
			   'good_comment_cnt' => 0,
			   'bad_comment_cnt' => 0,
			 ),
		array( 'company_id' => 5,
		       'google_cnt' => 123000, 
			   'baidu_cnt' => 9400,
			   'good_comment_cnt' => 0,
			   'bad_comment_cnt' => 0,
			 ),
	);

	foreach ($household_info as $row) {

		$fields = array(
		     'company_id' => $row['company_id'],
		     'google_cnt' => $row['google_cnt'],
		     'baidu_cnt' => $row['baidu_cnt'],
		     'good_comment_cnt' => $row['good_comment_cnt'],
		     'bad_comment_cnt' => $row['bad_comment_cnt'],
			 );
		if (!empty($row['bad_comment_links'])) {
			$fields['bad_comment_links'] = $row['bad_comment_links'];
		}

		if (!empty($row['good_comment_links'])) {
			$fields['good_comment_links'] = $row['good_comment_links'];
		}

		$id = db_insert('youwo_household_info')
			->fields($fields)
			->execute();
	}
}

function remove_data() {
   $num_deleted = db_delete('youwo_household_info')
       ->execute();

   $num_deleted = db_delete('youwo_household_basic')
       ->execute();

   $num_deleted = db_delete('youwo_company')
       ->execute();
}
	
function youwo_install_data() {
	remove_data();
	insert_youwo_company();
	insert_household_basic();
	insert_household_info();
	return "";
}
