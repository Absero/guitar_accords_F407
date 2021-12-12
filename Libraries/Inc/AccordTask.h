#ifndef INC_ACCORDTASK_H_
#define INC_ACCORDTASK_H_

typedef struct {
	char chord[4];
	float time;
} accordInfo_t;

void StartAccordTask(void *argument);

#endif /* INC_ACCORDTASK_H_ */
