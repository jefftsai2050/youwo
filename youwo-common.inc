<?php

function get_request_status_str ($s)
{
	$status = '';
	switch ($s) {
	  case 0:
	     $status = '关闭';
		 break;
	  case 1:
	     $status = '等待处理中...';
		 break;
	  case 2:
	     $status = '正在处理中...';
		 break;
	  case 3:
	     $status = '已处理';
		 break;
	  case 10:
	     $status = '处理成功';
		 break;
	  case 11:
	     $status = '处理失败';
		 break;
	}

	return $status;
}
?>
