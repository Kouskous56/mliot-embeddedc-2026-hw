// HW01: HỆ THỐNG GIÁM SÁT LÒ NHIỆT CÔNG NGHIỆP
// Kouskous56; Nguyễn Đăng Khoa; 2551799

#include <stdio.h>
#include <stdint.h>

// TASK 1: POINTERS & MEMORY

void parse_config(const uint8_t *config_packet, int16_t *high_threshold) {
	// HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

  	// Kiểm tra chống NULL
	if (config_packet == NULL || high_threshold == NULL) {
      		return;
  	}
	// Gộp 2 byte theo Litlle-Endian
	// Dịch trái byte MSB [1] đi 8 bit rồi dùng phép OR với byte LSB [0]
	// Ép thành kiểu int16_t để lấy dấu
   	*high_threshold = (int16_t)(config_packet[0] | (config_packet[1] << 8));

	// HỌC VIÊN KẾT THÚC VIẾT CODE
}

// TASK 2: COMPILER & VOLATILE

int16_t read_temperature_reg(void *hw_sensor_reg) {
    	// HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

	// Kiểm tra con trỏ
	if (hw_sensor_reg == NULL) {
		return 0;
	}
	// Ép kiểu con trỏ sang volatile int16_t*
	volatile int16_t *reg = (volatile int16_t *)hw_sensor_reg;
	// Trả về giá trị nhiệt độ hiện tại
	return *reg;

    	// HỌC VIÊN KẾT THÚC VIẾT CODE
}

// TASK 3: DATA TYPES & BITWISE OPERATIONS

void control_output(uint8_t *control_reg, uint8_t fan_enable, uint8_t alarm_enable) {
    	// HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

	// Kiểm tra con trỏ
	if (control_reg == NULL) {
		return;
	}
	// Bit 0: fan; Bit 1: alarm
	// Xóa về 0 2 bit 0 và 1, giữ nguyên các bit còn lại
	*control_reg &= ~0x03;	// 1111 1100
	// Nếu fan == 1, bật bit 0
	if (fan_enable == 1) {
		*control_reg |= 0x01; // 0000 0001
	}
	// Nếu fan == 0, giữ nguyên
	// Nếu alarm == 1, bật bit 1
	if (alarm_enable == 1) {
		*control_reg |= 0x02; // 0000 0010
	}
	// Nếu alarm == 0, giữ nguyên

    	// HỌC VIÊN KẾT THÚC VIẾT CODE
}

// HÀM MAIN KIỂM TRA (Học viên giữ nguyên để chạy thử nghiệm, chỉ thay đổi input nếu cần)

int main() {

    // 1. Test Task 1
    int16_t threshold = 0;
    uint8_t mock_packet[2] = {0x64, 0x00};
    parse_config(mock_packet, &threshold); // Có thể thay mock_packet thành NULL để test chống crash khi gặp NULL
    printf("Threshold Parsed: %d C\n", threshold);

    // 2. Test Task 2
    volatile int16_t mock_hardware_sensor = 105; 
    int16_t current_temp = read_temperature_reg((void*)&mock_hardware_sensor);
    printf("Current Temp Read: %d C\n", current_temp);

    // 3. Test Task 3
    uint8_t system_control_register = 0xFC; // 1111 1100
    if (current_temp >= threshold) {
        // Yêu cầu: fan = 1, alarm = 0
        control_output(&system_control_register, 1, 0);
    }
    printf("Control Reg Output: 0x%02X\n", system_control_register);

    return 0;
}
// Threshold Parsed: 100 C
// Current Temp Read: 105 C
// Control Reg Output: 0xFD