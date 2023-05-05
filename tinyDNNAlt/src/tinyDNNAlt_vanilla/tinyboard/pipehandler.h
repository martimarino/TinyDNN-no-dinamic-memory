#pragma once
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

namespace tinyboard {
		class TopicHandler {
			std::string name_;
		public:

			TopicHandler(std::string name) {
				name_ = name;
			}

			bool make() {
				int res = mkfifo(name_.c_str(),0666);
				return !res;
			}

			template <class T>
			T readObj() {
				int fd = open(name_.c_str(),O_RDONLY);
				T tmp;
				int cc = read(fd,&tmp,sizeof(T));
				return tmp;
			}

			template <class T>
			bool writeObj(T data) {
				int fd = open(name_.c_str(),O_WRONLY);
				int cc = write(fd,&data,sizeof(T));
				return cc == sizeof(T);
			}
		};
}