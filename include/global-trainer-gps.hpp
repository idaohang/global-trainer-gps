#ifndef GLOBAL_TRAINER_GPS
#define GLOBAL_TRAINER_GPS
#include <libusb.h>
#include <string>
#include <cstdint.h>

namespace GlobalTrainerGPS {

	constexpr uint16_t GLOBAL_TRAINER_VENDOR_ID           = 0x0483;
	constexpr uint16_t GLOBAL_TRAINER_PRODUCT_ID          = 0x5740;
	constexpr unsigned char GLOBAL_TRAINER_ENDPOINT_WRITE = 0x03;
	constexpr unsigned char GLOBAL_TRAINER_ENDPOINT_READ  = 0x81;
	constexpr int                           READ_BUF_SIZE = 2096;

	struct Sample {
		char pad1; // 01
		uint16_t heartrate;
		char pad2; // 02
		int32_t latitude;  /* fixed point * 10^6 */
		int32_t longitude; /* fixed point * 10^6 */
		int16_t altitude;

		int16_t gps_speed; 		/* 906  = 2.51667
				                   1061 = 2.94722
				                   1113 = 3.09167
				                   1141 = 3.16944
				                   So.. multiply by 0.002777781 */
		int16_t compass;
		int16_t distance_period; /* how much distance was traveled in
		                            this sample. 628 = 6.28 */
		int16_t time_period; /* 200 = 2.00 seconds" */
		int16_t unknown; /* time period may be 4 bytes little endian */
		char pad3; // 01
		char pad4; // 03
		char pad5; // 03
		char pad6; // 00
		char pad7; // 00
		char pad8; // 00
		char pad9; // 03
		char pad10; // 00
		char pad11; // 04
		char pad12; // 00
		char pad13; // 00
		char pad14; // 00
		char pad15; // 03
		char pad16; // 00
		char pad17; // 05
		char pad18; // 00
		char pad19; // 00
		char pad20; // 00
		char pad21; // 00
		char pad22; // 00
		char pad23; // 03
		char pad24; // 00
		int16_t distance_total;
	}

	class GlobalTrainer {
	public:
		GlobalTrainer();
		GlobalTrainer(const GlobalTrainer&) = delete;
		GlobalTrainer operator=(const GlobalTrainer&) = delete;
		~GlobalTrainer();

		void init();
		void first();
		void second();
		void third();
		void fourth();
		void fifth();
		void get_first_record();

	private:
		int  do_read(const std::string&);
		bool do_write(unsigned char *buf, int size, const std::string&);

		libusb_context       *usb_ctxt;
		libusb_device_handle *usb_handle;
		bool                  interface_claimed;
		bool                  detached_kernel;
		unsigned char        *read_buf;
	};

}

#endif
