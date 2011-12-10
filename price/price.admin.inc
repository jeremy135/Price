<?php 
function price_show_val() {
	$query = db_select('j_val','jv')->fields('jv');
		
	$header = array(t('#'), t('val'), t('Actions'));
		
	$rows = array();
	$count_query = clone $query;
	$count = $count_query->countQuery()->execute()->fetchField();
	$output = '';
	$result = $query->execute();
	if ($count > 0) {
		$i = 0;
		foreach($result as $item) {
			$i++;
			$row = array();
			$row[] = $i;
			$row[] = $item->val;
			$row[] =    l(('edit'), 'admin/price/val/edit/' . $item->vid) .'&#09;&#09;&#09;'. l(('delete'), 'admin/price/val/del/' . $item->vid);
			$rows[] = $row;
		}
	$output = theme('table', array('header' => $header, 'rows' => $rows, array('class' => array('val_table'))));
	} else {
		$output = t('No val');
		//$output .= l('add val', 'admin/price/val/add');
	}
	/*$output = 'Testing information';*/
	return $output;
}

function price_add_val_form() {
	$form['val'] = array(
		'#title' => t('val'),
		'#type' => 'textfield',
		'#required' => TRUE,	
	);
	$form['submit'] = array (
		'#type' => 'submit',
		'#value' => 'Add',
	);
	return $form;
}
function price_add_val_form_validate($form, &$form_state) {
	$val = $form_state['values']['val'];
	$result = db_select('j_val','jv')
			->fields('jv')
			->condition( 'jv.val', $val, '=')
			->countQuery()
  			->execute()
  			->fetchField();
	if($result > 0) {
		form_set_error('', t('This val exist.'));
	}
	
}
function price_add_val_form_submit($form, &$form_state) {
	$val = $form_state['values']['val'];
	$fields = array('val'=> $val);
	db_insert('j_val')->fields($fields)->execute();
	drupal_set_message('Val has added');
	$form_state['redirect'] = 'admin/price/val';
}
function price_delete_val($vid){
    $form = drupal_get_form('price_val_delete_form', $vid);
    return drupal_render($form);
}

function price_val_delete_form($form, &$form_state, $vid) {
	$form = array();
	$form['vid'] = array(
		'#type' => 'hidden',
		'#value' => $vid,
	);
	$confirm = confirm_form($form, t('Вы действительно хотите удалить эту запись ?'), 'admin/price/val', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
	return $confirm;
}

function price_val_delete_form_submit($form, &$form_state) {
	$form = array();
	$vid = $form_state['values']['vid'];
	
	db_delete('j_val')->condition('vid', $vid, '=')->execute();
	drupal_set_message(t('Record have deleted'));
	$form_state['redirect'] = 'admin/price/val';
}
/*function show_cat() {
	$query = db_select('j_category','jv')->fields('jv');
		
	$header = array(t('#'), t('cat'), t('Actions'));
		
	$rows = array();
	$count_query = clone $query;
	$count = $count_query->countQuery()->execute()->fetchField();
	$output = '';
	$result = $query->execute();
	if ($count > 0) {
		$i = 0;
		foreach($result as $item) {
			$i++;
			$row = array();
			$row[] = $i;
			$row[] = $item->c_name;
			$row[] = l(('delete'), 'admin/price/cat/del/' . $item->tid);
			$rows[] = $row;
		}
	$output = theme('table', array('header' => $header, 'rows' => $rows, array('class' => array('cat_table'))));
	} else {
		$output = t('No cat');
		//$output .= l('add val', 'admin/price/val/add');
	}
	//$output = 'Testing information';
	return $output;
}*/
function price_show_cat_form() {
    $query = db_select('j_category','jc')->fields('jc')->orderBy('weight', 'ASC');
    //dsm($result);
    //$questions =
	$count_query = clone $query;
	$count = $count_query->countQuery()->execute()->fetchField();
	
	
	$result = $query->execute();
	
    foreach ($result as $value) {
		$cat[] = $value;
	}
	if(!isset($cat)) {
		$form['no_category'] = array(
			'#markup' => t('No categories'),
		);
		return $form;
	}
		$delta = round(count($cat) / 2);
	
		$form['result'] = array(
			'#tree' => TRUE,
		);
	
		foreach ($cat as $tid => $item) {
			$form['result'][$item->tid]['#item'] = (object) array(
						'tid' => $item->tid,
						'c_name' => $item->c_name,
						'weight' => $item->weight,
						'status' => $item->status,
			);
			$form['result'][$item->tid]['#weight'] = $item->weight;
			$form['result'][$item->tid]['weight'] = array(
				'#type' => 'weight',
				'#title' => t('Weight for @title', array('@title' => $item->c_name)),
				'#title_display' => 'invisible',
				'#delta' => $delta,
				'#default_value' => $item->weight,
				'#attributes' => array('class' => array('selected-weight')),
			);
			$form['result'][$item->tid]['status'] = array (
				'#type' => 'checkbox', 
				'#default_value' => $item->status, 
			);
		}
		
		$form['actions'] = array('#type' => 'actions');
		$form['actions']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Save order'),
		); 
	
	
	return $form;
}
function price_show_cat_form_submit($form, &$form_state) {
    $varables = $form_state['values']['result'];
    foreach ($varables as $tid => $item) {

		db_update('j_category')
			->condition('tid', $tid, '=')
			->fields(array(
				'weight' => $item['weight'],
                'status' => $item['status'],
			))
			->execute();
	}
    drupal_set_message('Порядок сохранен');
}
function theme_price_show_cat_form($variables) {

	$form = $variables['form'];
	if(!isset($form['result'])) {
		return ;
	}
		$header = array(t('Category'), t('Weight'), t('Published'), t('Operations'));
		$i = 0;
		$rows = array();
		foreach (element_children($form['result']) as $tid) {
			$c_name = $form['result'][$tid]['#item']->c_name;
			$status = $form['result'][$tid]['#item']->status;
			$tid = $form['result'][$tid]['#item']->tid;
			$bg_color = 'background-color:#BBDDFF';
			
			$row = array();
			$row[] = $c_name;
			$row[] = drupal_render($form['result'][$tid]['weight']);
			$row[] = drupal_render($form['result'][$tid]['status']);
			$row[] =  l(('edit'), 'admin/price/cat/edit/' . $tid) . '&#32; &#32;&#32;' . l(('delete'), 'admin/price/cat/del/' . $tid);
			
			$rows[] = array('data' => $row, 'class' => array('draggable', 'row-' . $tid), 'sticky' => FALSE/*, 'style' =>$bg_color*/);
			$i++;
		}
	
		drupal_add_tabledrag('selected', 'order', 'sibling', 'selected-weight');
	
		$output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'selected')));
		$output .= drupal_render_children($form);
	
	return $output;
}

function price_add_cat_form() {
	$form['cat'] = array(
		'#title' => t('cat'),
		'#type' => 'textfield',
		'#required' => TRUE,	
	);
	$form['submit'] = array (
		'#type' => 'submit',
		'#value' => 'Add',
	);
	return $form;
}
function price_add_cat_form_validate($form, &$form_state) {
	$name = $form_state['values']['cat'];
	$result = db_select('j_category','jv')
			->fields('jv')
			->condition( 'jv.c_name', $name, '=')
			->countQuery()
  			->execute()
  			->fetchField();
	if($result > 0) {
		form_set_error('', t('This cat exist.'));
	}
	
}
function price_add_cat_form_submit($form, &$form_state) {
	$cat = $form_state['values']['cat'];
	$fields = array('c_name'=> $cat);
	db_insert('j_category')->fields($fields)->execute();
	drupal_set_message(t('Cat has added'));
	$form_state['redirect'] = 'admin/price/cat';
}
function price_delete_cat($tid){
	$form = drupal_get_form('price_cat_delete_form', $tid);
    return drupal_render($form);
}

function price_cat_delete_form($form, &$form_state, $tid) {
	$form = array();
	$form['tid'] = array(
		'#type' => 'hidden',
		'#value' => $tid,
	);
	$confirm = confirm_form($form, t('Вы действительно хотите удалить эту запись ?'), 'admin/price/cat', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
	return $confirm;
}

function price_cat_delete_form_submit($form, &$form_state) {
	$form = array();
	$tid = $form_state['values']['tid'];
	
	db_delete('j_category')->condition('tid', $tid, '=')->execute();
	drupal_set_message(t('Record have deleted'));
	$form_state['redirect'] = 'admin/price/cat';
}
function price_add_product_form() {
	$form['name'] = array(
		'#title' => t('name'),
		'#type' => 'textfield',
		'#required' => TRUE,	
	);
	$form['cost'] = array(
		'#title' => t('cost'),
		'#type' => 'textfield',
		'#required' => TRUE,	
	);
	//$options1['0'] = t('Choose Category');
	$result = db_select('j_category','jv')->fields('jv')->execute();
	foreach($result as $item) {
		$options1[$item->tid] = $item->c_name;
	}
	$form['cat'] = array (
		'#type' => 'select',
		'#title' => t('Category'),
		'#options' => ($options1) ? $options1 : '',
		'#required' => TRUE,	
	);
	//$options2['0'] = t('Choose Val');
	$result = db_select('j_val','jv')->fields('jv')->execute();
	foreach($result as $item) {
		$options2[$item->vid] = $item->val;
	}
	$form['vid'] = array (
		'#type' => 'select',
		'#title' => t('Val'),
		'#options' => ($options2) ? $options2 : '',
		'#required' => TRUE,	
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Add Product'), 
	);
	return $form;
}
function price_add_product_form_validate($form, &$form_state) {
/*	$cat = $form_state['values']['cat'];
	$val = $form_state['values']['val'];	
	if($cat == 0) {
		form_set_error('cat', t('Choose Category.'));
		}
	if($val == 0 ){
		form_set_error('val', t('Choose Val.'));
	}*/
}
function price_add_product_form_submit($form, &$form_state) {
	$fields = array (
		'name' => $form_state['values']['name'],
		'cost' => $form_state['values']['cost'],
		'tid' => $form_state['values']['cat'],
		'vid'=> $form_state['values']['vid'],
	);
	db_insert('j_price')->fields($fields)->execute();
	drupal_set_message(t('Product has added'));
	$form_state['redirect'] = 'admin/price';
}
function price_edit_val_form($form, &$form_state, $vid) {
	$result = db_select('j_val', 'jv')->fields('jv')->condition('vid', $vid, '=')->execute()->fetchObject();
	
	$form['val'] = array(
		'#title' => t('val'),
		'#type' => 'textfield',
		'#required' => TRUE,
		'#default_value' => $result->val,	
	);
	$form['vid'] = array(
		'#type' => 'hidden',
		'#value' => $result->vid,	
	);
	$form['submit'] = array (
		'#type' => 'submit',
		'#value' => 'Save',
	);
	return $form;
}
function price_edit_val_form_submit($form, &$form_state) {
	///dsm($form_state);
    $val = $form_state['values']['val'];
    $vid = $form_state['values']['vid'];
    $fields = array (
        'val'=> $val,   
    );
    db_update('j_val')->condition('vid', $vid, '=')->fields($fields)->execute();
    drupal_set_message(t('Val has saved'));
    $form_state['redirect'] = 'admin/price/val';
}
function price_show_form($form, &$form_state, $cat) {
  //dsm($cat);
  /* $result = db_select('j_price','jp')
                   ->fields('jp',array('vid'))
                   ->addField('jv', 'val')
                   ->addJoin('inner','j_val', 'jv', 'jp.vid = jv.vid')
                   ->condition('tid', $cat,'=')
                   ->orderBy('weight','ASC')
                   ->execute();*/
    $result = db_query("SELECT jp.*, jv.val FROM {j_price} jp INNER JOIN {j_val} jv ON jp.vid = jv.vid WHERE jp.tid = :tid ORDER BY jp.weight ASC", array(':tid' => $cat));
    $cat_name = db_select('j_category','jc')->condition('tid', $cat, '=')->fields('jc', array('c_name'))->execute()->fetchField();
  
    foreach ($result as $value) {
		$products[] = $value;
	}
	$delta = round(count($products) / 2);

	$form['product'] = array(
		'#tree' => TRUE,
	);

	foreach ($products as $pid => $item) {
		$form['product'][$item->pid]['#item'] = (object) array(
					'pid' => $item->pid,
					'name' => $item->name,
					'weight' => $item->weight,
					'status' => $item->status,
                    'cost' => $item->cost,
                    'val'=> $item->val,
		);
		$form['product'][$item->pid]['#weight'] = $item->weight;
		$form['product'][$item->pid]['weight'] = array(
			'#type' => 'weight',
			'#title' => t('Weight for @title', array('@title' => $item->name)),
			'#title_display' => 'invisible',
			'#delta' => $delta,
			'#default_value' => $item->weight,
			'#attributes' => array('class' => array('selected-weight')),
		);
        $form['product'][$item->pid]['status'] = array (
            '#type' => 'checkbox',  
            '#default_value' => $item->status, 
        );
	}
   $form['value'] = array (
        '#type' => 'hidden',
        '#value'=> $cat_name,
    );
    $form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save order'),
	);

	return $form;
    
}
function price_show_form_submit($form, &$form_state) {
     $varables = $form_state['values']['product'];
     foreach ($varables as $pid => $item) {

    db_update('j_price')
			->condition('pid', $pid, '=')
			->fields(array(
				'weight' => $item['weight'],
                'status' => $item['status'],
			))
			->execute();
	}	
}
function theme_price_show_form($variables) {

	$form = $variables['form'];
    
	$header = array(t('Product'), t('Weight'), t('Published'),t('Cost'), t('Operations'));
    $caption = $form['value']['#value'];
	$i = 0;
	$rows = array();
	foreach (element_children($form['product']) as $pid) {
		$name = $form['product'][$pid]['#item']->name;
		$status = $form['product'][$pid]['#item']->status;
        $pid = $form['product'][$pid]['#item']->pid;
        $cost = $form['product'][$pid]['#item']->cost;
        $val = $form['product'][$pid]['#item']->val;
        
		$bg_color = 'background-color:#BBDDFF';
		//$pu = ($status) ? l('Отключить', "sfaq/do/unpublish/$qid") : l('Включить', "sfaq/do/publish/$qid");
		
		$row = array();
		$row[] = $name;
		$row[] = drupal_render($form['product'][$pid]['weight']);
        $row[] = drupal_render($form['product'][$pid]['status']);
        $row[] = $cost.'<span class="val">'.$val .'</span>';
		$row[] = l(('delete'), 'admin/price/pro/del/' . $pid);
        
		$rows[] = array('data' => $row, 'class' => array('draggable', 'row-' . $pid), 'sticky' => FALSE/*, 'style' =>$bg_color*/);
		$i++;
	}

	drupal_add_tabledrag('selected', 'order', 'sibling', 'selected-weight');

	$output = theme('table', array('header' => $header, 'rows' => $rows, 'caption' => $caption, 'attributes' => array('id' => 'selected')));
	$output .= drupal_render_children($form);

	return $output;
}
function price_edit_cat_form ($form, &$form_state, $tid) {
    $result = db_select('j_category', 'jc')->fields('jc')->condition('tid', $tid, '=')->execute()->fetchObject();
    $form['cat'] = array(
		'#title' => t('cat'),
		'#type' => 'textfield',
		'#required' => TRUE,
        '#default_value' => $result->c_name,	
	);
    $form['tid'] = array (
        '#type' => 'hidden',
        '#value' => $result->tid,
    );
	$form['submit'] = array (
		'#type' => 'submit',
		'#value' => 'Save',
	);
	return $form;
}
function price_edit_cat_form_submit ($form, &$form_state) {
    //dsm($form_state);
    $c_name = $form_state['values']['cat'];
    $tid = $form_state['values']['tid'];
    $fields = array (
        'c_name'=> $c_name,   
    );
    db_update('j_category')->condition('tid', $tid, '=')->fields($fields)->execute();
    drupal_set_message(t('Category has saved'));
    $form_state['redirect'] = 'admin/price/cat';
}
function price_edit_product_form($form, &$form_state, $pid) {
    $result1 = db_select('j_price', 'jp')->fields('jp')->condition('pid', $pid, '=')->execute()->fetchObject();
    //dsm($result1);
	$form['name'] = array(
		'#title' => t('name'),
		'#type' => 'textfield',
		'#required' => TRUE,
        '#default_value' => $result1->name,	
	);
	$form['cost'] = array(
		'#title' => t('cost'),
		'#type' => 'textfield',
		'#required' => TRUE,
        '#default_value' => $result1->cost,	
	);
	//$options1['0'] = t('Choose Category');
	$result = db_select('j_category','jv')->fields('jv')->execute();
	foreach($result as $item) {
		$options1[$item->tid] = $item->c_name;
	}
	$form['cat'] = array (
		'#type' => 'select',
		'#title' => t('Category'),
		'#options' => $options1,
		'#required' => TRUE,
        '#default_value' => $result1->tid,	
	);
	//$options2['0'] = t('Choose Val');
	$result = db_select('j_val','jv')->fields('jv')->execute();
	foreach($result as $item) {
		$options2[$item->vid] = $item->val;
	}
	$form['vid'] = array (
		'#type' => 'select',
		'#title' => t('Val'),
		'#options' => $options2,
		'#required' => TRUE,
        '#default_value' => $result1->vid,	
	);
    $form['pid'] =array (
        '#type' => 'hidden',
        '#value' => $result1->pid,
    );
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'), 
	);
	return $form;
}
function price_edit_product_form_submit($form, &$form_state) {
     $pid = $form_state['values']['pid'];
     db_update('j_price')
			->condition('pid', $pid, '=')
			->fields(array(
				'name' => $form_state['values']['name'],
                'tid' => $form_state['values']['cat'],
                'vid' => $form_state['values']['vid'],
                'cost' => $form_state['values']['cost'],
			))
			->execute();
		
    drupal_set_message('Product has saved');
    $form_state['redirect'] = 'admin/price';
}
function price_delete_form($form, &$form_state, $pid) {
	$form = array();
	$form['pid'] = array(
		'#type' => 'hidden',
		'#value' => $pid,
	);
	$confirm = confirm_form($form, t('Вы действительно хотите удалить эту запись ?'), 'admin/price', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
	return $confirm;
}

function price_delete_form_submit($form, &$form_state) {
	$form = array();
	$pid = $form_state['values']['pid'];
	
	db_delete('j_price')->condition('pid', $pid, '=')->execute();
	drupal_set_message(t('Record have deleted'));
	$form_state['redirect'] = 'admin/price';
}
