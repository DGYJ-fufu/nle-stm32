/* Includes ------------------------------------------------------------------*/

#ifndef __HAL_KEY_H
#define __HAL_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#define	FAN_CTL_GPIO		GPIOA
#define	FAN_CTL_GPIO_PIN	GPIO_PIN_1
#define	LIGHT_CTL_GPIO		GPIOA
#define	LIGHT_CTL_GPIO_PIN	GPIO_PIN_8

#define	KEY2_GPIO		GPIOC
#define	KEY2_GPIO_PIN	GPIO_PIN_13
#define	KEY3_GPIO		GPIOB
#define	KEY3_GPIO_PIN	GPIO_PIN_1
#define	KEY4_GPIO		GPIOB
#define	KEY4_GPIO_PIN	GPIO_PIN_5

#define KEY_UP			1
#define KEY_DOWN		0

void light_ctl_init(void);
void fan_ctl_init(void);
void fan_ctl(char is_fan_on);
void light_ctl(char is_light_on);
void scan_keys(void);
void keys_init(void);
char isKey2Pressed(void);
char isKey3Pressed(void);
char isKey4Pressed(void);
void resetKey2(void);
void resetKey3(void);
void resetKey4(void);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_KEY_H */


