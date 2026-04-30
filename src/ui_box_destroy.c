void ui_box_destroy(ui_box_t* box) {
    if (box) {
        free(box);
    }
	ui_box_t *current = box->child_boxes;
	while(current) {
		ui_box_t *next = current->next; 
		ui_box_destroy(current);
		current = next;
	}
}
