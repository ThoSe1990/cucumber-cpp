#include <iostream>
#include "vm.hpp"
#include "compiler.hpp"
#include "chunk.hpp"
namespace cwt::details
{

void vm::interpret(std::string_view source)
{
  compiler c(source);
  function feature = c.compile();

  m_stack.push(std::move(feature));
  call(m_stack.top().as<function>());
  run();
}

void vm::call(const function& func)
{
  m_frames.emplace_back(call_frame{func.chunk_ptr.get()});
}

void vm::run()
{
  call_frame frame = m_frames.back();
  auto it = frame.chunk_ptr->cbegin();
  for (;;)
  {
    switch (it.next_as_instruction())
    {
      case op_code::constant:
      {
        uint32_t next = it.next();
        std::cout << "op_code::constant: "
                  << static_cast<int>(frame.chunk_ptr->constant(next).type())
                  << std::endl;
        m_stack.push(std::move(frame.chunk_ptr->constant(next)));
      }
      break;
      case op_code::define_var:
      {
        uint32_t next = it.next();
        std::string name =
            frame.chunk_ptr->constant(next).copy_as<std::string>();
        m_globals[name] = m_stack.top();
        m_stack.pop();

        std::cout << "op_code::define_var globals: "
                  << static_cast<int>(m_globals[name].type()) 
                  << std::endl;
      }
      break;
      case op_code::get_var:
      {
        uint32_t next = it.next();
        std::string name =
            frame.chunk_ptr->constant(next).copy_as<std::string>();
        std::cout << "op_code::get_var globals: "
                  << static_cast<int>(m_globals[name].type()) 
                  << std::endl;
        m_stack.push(m_globals[name]);
      }
      break;
      case op_code::call:
      {
        std::cout << "op_code::call stack top: "
                  << static_cast<int>(m_stack.top().type()) 
                  << std::endl;

        uint32_t argc = it.next();  // this is arg count
        call(m_stack.top().as<function>());
        frame = m_frames.back();
        it = frame.chunk_ptr->cbegin();
      }
      break;
      case op_code::func_return:
      {
        std::cout << "func_return" << std::endl;
        m_frames.pop_back();
        if (m_frames.empty())
        {
          m_stack.pop();
          return;
        }
        else
        {
          frame = m_frames.back();
        }
      }
      break;
      default:
      {
        std::cout << "default!" << std::endl;
      }
    }
  }
}

}  // namespace cwt::details