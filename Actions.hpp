/*
Title : Actions
Author: Edward R. Gonzalez

Description: This was a little experiment of mine to mess with action binding...

Allows for non-member functions to be binded to an action, implements a functioning queue as well.

TODO: Possibly add support for member functions. Have it so that deduction of delegate typef is not required to add to queue properly (right now it does, see input procedure for example);
*/


#pragma once

#include "Cpp_Alias.hpp"



namespace Actions
{
	struct IAction
	{
		virtual void DoAction() = NULL;
	};

	template<typename FunctionType, typename... ActionParams>
	struct AAction : IAction
	{
		using ActionType = Delegate< FunctionType >;

	public:
		AAction(ActionType _actionToAssign, ActionParams... _params) :
			action(_actionToAssign),
			params(_params...)
		{};

	private:
		using IndexType = DataSize;

		void DoAction_Implementation(ActionParams... _params) { action(_params...); }

		template<IndexType... TuplePackIndex>                                                            // TuplePackSequence<TuplePackIndex...>
		void ExpandTuple_CallDoActionImplementaiton(const Ref(Tuple<ActionParams...>) _paramsToExpand, std::index_sequence   <TuplePackIndex...>)
		{
			// ExpandTuplePack<TuplePackIndex>
			DoAction_Implementation(std::get       <TuplePackIndex>(_paramsToExpand)...);
		}

		Tuple<ActionParams...> params;

		ActionType action;

	public:   // IAction

		virtual void DoAction() override
		{
			ExpandTuple_CallDoActionImplementaiton
			(
				params,
				// MakeTuplePackSequence  <ActionParams...>()
				std::index_sequence_for<ActionParams...>()
			);
		};
	};


	struct ActionQueue
	{
		sfn HasAction()
		{
			return actionQueue.size() > 0;
		}

		template<typename FunctionType, typename... ActionParams>
		sfn AddToQueue(Delegate< FunctionType> _actionToQueue, ActionParams... _paramsForAction)
		{
			// This is extremely inefficient, but in order to fix requires an object pool or something else...
			SPtr< AAction<FunctionType, ActionParams...> > ptrToAction = MakeSPtr< AAction<FunctionType, ActionParams...> >(_actionToQueue, _paramsForAction...);

			if (HasAction())
			{
				bool found = false;

				using Element = decltype(actionQueue.begin());

				for (Element element = actionQueue.begin(); element != actionQueue.end(); element++)
				{
					if ((*element).get() == ptrToAction.get())
					{
						found = true;
					}
				}

				if (not found)
				{
					actionQueue.push_front(std::move(ptrToAction));
				}
			}
			else
			{
				actionQueue.push_front(std::move(ptrToAction));
			}
		}

		sfn DoNextAction()
		{
			if (actionQueue.size() > 0)
			{
				actionQueue.back()->DoAction();

				actionQueue.pop_back();
			}
		}

		using QueueType = std::deque< SPtr<IAction>>;

		QueueType actionQueue;
	};
}
