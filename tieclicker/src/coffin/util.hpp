#pragma once

#include<deque>
#include<functional>

namespace cfn {
	class mono_scheduler {
	public:
		void update() {
			if (list_.empty())
				return;

			list_.front()();
			list_.pop_front();
		}

		bool has_event()const {
			return !list_.empty();
		}

		void push(std::function<void()> const&ev) {
			list_.push_back(ev);
		}

	private:
		std::deque<std::function<void()>> list_;
	};

	class mainloop_scheduler {
	public:
		void update() {
			if (list_().empty())
				return;

			list_().front()();
			list_().pop_front();
		}

		bool has_event()const {
			return !list_().empty();
		}

		void push(std::function<void()> const&ev) {
			list_().push_back(ev);
		}

	private:
		static std::deque<std::function<void()>>& list_() {
			static std::deque<std::function<void()>> list;
			return list;
		}
	};

	template<class type>
	class basic_cnannel {
	public:
		using event_type = std::function<void(type)>;

		basic_cnannel() {}

		void on_next(type arg) {
			mainloop_scheduler scheduler;
			for (auto ev : event_list_) {
				scheduler.push(
					[=] {ev(arg); }
				);
			}

		}

		void subscribe(event_type const&ev) {
			event_list_.push_back(ev);
		}

	private:
		std::vector<event_type> event_list_;
	};
}
