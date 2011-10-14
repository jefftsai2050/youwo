<table id="youwo-my-score">
  <tbody>
    <tr>
      <td class="title">
           用户
      </td>
      <td colspan="3"><?php print $user->name ?></td>
	</tr>
	<tr>
	  <td>
	      积分
      </td>
	  <td> <?php print $youwo_user->score ?> </td>
	</tr>
	<tr>
	  <td>
	      帐户类新
	  </td>
	  <td> <?php switch ($youwo_user->type) {
	          case 0:
			    print "普通";
				break;
			  case 1: 
			    print "中级"; 
				break;
			  case 2: 
			    print "高级";
				break;
			} ?> </td>
  </tbody>
</table>
