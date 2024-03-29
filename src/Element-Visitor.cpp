/**
 * @file   Simple-XML/src/Element-Visitor.cpp
 * @date   Started 2018-08-14
 * @author Lester J. Dowling
 */

#include "Simple-XML/Element-Visitor.hpp"

#define TRACE_VISITOR
#undef TRACE_VISITOR
#ifdef TRACE_VISITOR
#define __TRACER(args) std::cout << args << std::endl;
#else // TRACE_VISITOR not defined
#define __TRACER(args)
#endif


#ifdef NDEBUG
#undef TRACE_VISITOR
#endif

namespace simple_xml {
	using std::string;
	using std::vector;
	using std::map;
	using std::optional;
	using std::function;

	bool Element_Visitor::visit_first_child()
	{
		__TRACER(
		  ">>> " << __FUNCTION__ << ": " << '(' << current().name() << ' '
				 << current().children.size() << " children" << ')' << path_to_string());
		if (current().children.empty())
			return false;
		m_current_index_path.push_back(m_current_index);
		m_current_index = current().children.front();
		return true;
	}


	bool Element_Visitor::resume_parent()
	{
		__TRACER(
		  ">>> " << __FUNCTION__ << ": " << '(' << current().name() << ')'
				 << path_to_string());
		if (this->is_current_index_root())
			return false;
		m_current_index = m_current_index_path.back();
		m_current_index_path.pop_back();
		return true;
	}


	bool Element_Visitor::visit_next_sibling()
	{
		if (this->is_current_index_root())
			return false;
		__TRACER(
		  ">>> " << __FUNCTION__ << ": " << '(' << parent().name() << ' '
				 << parent().children.size() << " children" << ')' << path_to_string());
		bool take_next = false;
		for (auto sibling_index : parent().children) {
			if (take_next) {
				m_current_index = sibling_index;
				return true;
			}
			if (m_current_index == sibling_index) {
				take_next = true;
			}
		}
		return false;
	}


	size_t Element_Visitor::depth() const
	{
		return m_elements.empty() ? 0 : m_current_index_path.size() + 1;
	}


	string Element_Visitor::path_to_string() const
	{
		if (m_elements.empty())
			return {};
		string result;
		Element_Path_Iterator nitr = m_current_index_path.begin();
		Element_Path_Iterator const nend = m_current_index_path.end();
		while (nitr != nend) {
			const Element::Index idx = *nitr++;
			result += m_elements[idx].name();
			result += " --> ";
		}
		result += current().name();
		return result;
	}


	void Element_Visitor::visit_all_depth_first(function<bool(Element_Visitor&)> callback)
	{
		if (m_elements.empty())
			return;
		m_current_index = 0;
		m_current_index_path.clear();
		if (visit_all_predicate()) {
			if (!callback(*this))
				return;
		}
		while (true) {
			while (visit_first_child()) {
				if (visit_all_predicate()) {
					if (!callback(*this))
						return;
				}
			}
			while (true) {
				if (visit_next_sibling()) {
					if (visit_all_predicate()) {
						if (!callback(*this))
							return;
					}
					break;
				}
				else { // No more siblings => retreat to parent:
					if (!resume_parent())
						return;
				}
			}
		}
	}
} // namespace simple_xml
