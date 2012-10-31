#include <global-trainer-gps.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

namespace GlobalTrainerGPS {
	GlobalTrainer::GlobalTrainer() :
		usb_ctxt(nullptr),
		usb_handle(nullptr),
		interface_claimed(false),
		detached_kernel(false)
	{
		int ecode = libusb_init(&usb_ctxt);
		if (ecode) {
			std::cerr << "Unable to init libusb" << std::endl;
			exit(EXIT_FAILURE);
		} else {
			libusb_set_debug(usb_ctxt, 3);
		}

		read_buf = static_cast<unsigned char*>(calloc(READ_BUF_SIZE, sizeof(unsigned char)));
	}

	GlobalTrainer::~GlobalTrainer() {
		free(read_buf);

		if (interface_claimed) {
			const int ecode = libusb_release_interface(usb_handle, 1);
			if (ecode) {
				std::cerr << "Error releasing interface: "
				          << libusb_error_name(ecode) << std::endl;
			} 
		}

		if (detached_kernel) {
			const int ecode = libusb_attach_kernel_driver(usb_handle, 1);
			if (ecode) {
				std::cerr << "Error reattaching kernel driver: "
				          << libusb_error_name(ecode) << std::endl;
			} 
		}
		
		const int ecode = libusb_reset_device(usb_handle);
		if (ecode && ecode != LIBUSB_ERROR_NOT_FOUND) {
			std::cerr << "Error reseting USB device: "
				          << libusb_error_name(ecode) << std::endl;
		}

		if (usb_handle) {
			libusb_close(usb_handle);
		}

		if (usb_ctxt) {
			libusb_exit(usb_ctxt);
		}
	}

	void GlobalTrainer::init() {
		int ecode = 0;
		usb_handle = libusb_open_device_with_vid_pid (usb_ctxt,
		                                              GLOBAL_TRAINER_VENDOR_ID,
		                                              GLOBAL_TRAINER_PRODUCT_ID);

		if (!usb_handle) {
			std::cerr << "Unable to open USB device" << std::endl;
			exit(EXIT_FAILURE);
		} 
		
		ecode = libusb_kernel_driver_active(usb_handle, 1);
		if (ecode == 1) {
			ecode = libusb_detach_kernel_driver(usb_handle, 1);
			if (ecode == 0)
				detached_kernel = true;
			else {
				std::cerr << "Libusb error detaching kernel driver: "
				          << libusb_error_name(ecode) << std::endl;
				exit(EXIT_FAILURE);
			}
		} else {
			switch (ecode) {
			case 0:
			case LIBUSB_ERROR_NOT_SUPPORTED:
				break;
			default:
				std::cerr << "Libusb error: "
				          << libusb_error_name(ecode) << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		ecode = libusb_claim_interface(usb_handle, 1);
		if (ecode) {
			std::cerr << "Unable to claim USB interface: "
			          << libusb_error_name(ecode) << std::endl;
			exit(EXIT_FAILURE);
		} 
		interface_claimed = true;
		std::cout << "Claimed interface!" << std::endl;
	}

	static void print_buf(const unsigned char* buf, const int size) {
		std::cerr << "Received " << std::dec << size << " bytes: \n\t";
		int j = 0;
		for (int i = 0; i < size; ++i) {
			std::cerr << std::hex << std::setw(2) << std::setfill('0')
			          << static_cast<short>(buf[i])
			          << std::dec;
			if (++j >= 0x10 && (i+1 < size)) {
				std::cerr << "\n\t";
				j = 0;
			}
			if (j > 0 && ((j % 4) == 0))
				std::cerr << " ";
		}
		std::cerr << std::endl;

		//std::string text(reinterpret_cast<const char*>(buf), size);
		//		std::cerr << std::endl << "As text: '" << text << "'" << std::endl;
	}

	int GlobalTrainer::do_read(const std::string &name) {
		int ecode;
		int transfered = 0;

		ecode = libusb_bulk_transfer(usb_handle, 
		                             GLOBAL_TRAINER_ENDPOINT_READ,
		                             read_buf,
		                             READ_BUF_SIZE,
		                             &transfered, 0);
		if (ecode) {
			std::cerr << "Error reading " << name << ": "
			          << libusb_error_name(ecode) << std::endl;
		}

		return transfered;
	}

	bool GlobalTrainer::do_write(unsigned char* buf, int size,
	                             const std::string &name) {
		int ecode;
		int transfered = 0;

		ecode = libusb_bulk_transfer(usb_handle, 
		                             GLOBAL_TRAINER_ENDPOINT_WRITE,
		                             buf, size, &transfered, 0);
		if (ecode) {
			std::cerr << "Error writing " << name << ": "
			          << libusb_error_name(ecode) << std::endl;
		} else {
			std::cerr << "Debug: " << name << " wrote " 
			          << transfered << "/" << size 
			          << " bytes." << std::endl;
			if ( transfered == size )
				return true;
		}

		return false;
	}

	void GlobalTrainer::first() {
		unsigned char out[] = { 0x02, 0x02, 0x00, 0x42, 0x01, 0x41 };
		int len_out = 6;

		if (do_write(out, len_out, "first"))
			do_read("first");
	}

	void GlobalTrainer::second() {
		unsigned char out[] = { 0x02, 0x01, 0x00, 0xBF, 0xBE };
		int len_out = 5;
		if (do_write(out, len_out, "second"))
			do_read("second");
	}

	void GlobalTrainer::third() {
		unsigned char out[] = { 0x02, 0x01, 0x00, 0x85, 0x84 };
		int len_out = 5;
		if (do_write(out, len_out, "third"))
			do_read("third");
	}

	void GlobalTrainer::fourth() {
		unsigned char out[] = { 0x02, 0x01, 0x00, 0x44, 0x45 };
		int len_out = 5;
		if (do_write(out, len_out, "fourth"))
			do_read("fourth");
	}

	void GlobalTrainer::fifth() {
		unsigned char out[] = { 0x02, 0x01, 0x00, 0x78, 0x79 };
		int len_out = 5;
		int read = 0;
		if (do_write(out, len_out, "fifth"))
			read = do_read("fifth");

		if (read > 0) {
			std::fstream file("fifth_msg", std::fstream::out | std::fstream::binary);
			for (int i = 0; i < read; i++) {
				file << read_buf[i];
			}
			file.close();

			print_buf(read_buf, read);
		}
	}

	void GlobalTrainer::get_first_record() {
		std::vector<unsigned char> record;
		unsigned char out[] = { 0x02, 0x05, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x84 };
		int len_out = 9;
		bool keep_reading = true;
		if (do_write(out, len_out, "get_first_record")) {
			do {
				int read = do_read("get_first_record");
				if ( read == 4 &&
				     read_buf[0] == 0x8A &&
				     read_buf[1] == 0x00 &&
				     read_buf[2] == 0x00 &&
				     read_buf[3] == 0x00 ) {
					keep_reading = false;
				} else {
					record.reserve(record.size() + read);
					for (int i = 0; i < read; i++) {
						record.push_back(read_buf[i]);
					}
					keep_reading = true;
				}

				if (keep_reading) {
					unsigned char buf[] = {0x02, 0x01, 0x00, 0x81, 0x80};
					keep_reading = do_write(buf, 5, "get_first_record continue");
				}
			} while (keep_reading);

			std::cerr << "Got a whole record of " << record.size() << " bytes." << std::endl;
			std::fstream file("first_record", std::fstream::out | std::fstream::binary);
			file.write(reinterpret_cast<char*>(record.data()), record.size());
			file.close();

		}
	}
}
