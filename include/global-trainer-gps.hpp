#ifndef GLOBAL_TRAINER_GPS
#define GLOBAL_TRAINER_GPS
#include <libusb.h>
#include <string>

namespace GlobalTrainerGPS {

	constexpr uint16_t GLOBAL_TRAINER_VENDOR_ID           = 0x0483;
	constexpr uint16_t GLOBAL_TRAINER_PRODUCT_ID          = 0x5740;
	constexpr unsigned char GLOBAL_TRAINER_ENDPOINT_WRITE = 0x03;
	constexpr unsigned char GLOBAL_TRAINER_ENDPOINT_READ  = 0x81;
	constexpr int                           READ_BUF_SIZE = 2096;

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
