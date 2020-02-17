/*
Title : Actions
Author: Edward R. Gonzalez

Description: This was a little experiment of mine to mess with action binding...

Allows for non-member functions to be binded to an action, implements a functioning queue as well.

TODO: Possibly add support for member functions. Have it so that deduction of delegate typef is not required to add to queue properly (right now it does, see input procedure for example);

Note: Right now due to type dynamics all actions are allocated via a pool and reuse is attempted by not guaranteed...
*/


#pragma once




#include "Cpp_Alias.hpp"



namespace Actions
{
	using IndexType = DataSize;



	struct IAction
	{
		virtual sfn DoAction() -> void = NULL;
	};

	template<typename FunctionType, typename... ActionParams>
	struct AAction : IAction
	{
		using ActionType = Delegate< FunctionType >;

	public:
		AAction(ActionType _actionToAssign, ActionParams... _params) :
			action(_actionToAssign),
			params(_params...     ),
			done  (false          )
		{};

		sfn Used() -> bool
		{
			return done;
		}

		sfn IsSame(ActionParams... _paramsForAction) -> bool
		{
			Tuple<ActionParams...> paramsToCheck(_paramsForAction...);

			if (params == paramsToCheck)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		sfn ReInitalize(ActionParams... _params)
		{
			params = Tuple<ActionParams...> (_params...);

			done = false;
		}

	private:
		sfn DoAction_Implementation(ActionParams... _params) { action(_params...); }

		template<IndexType... TuplePackIndex>                                                            // TuplePackSequence<TuplePackIndex...>
		sfn ExpandTuple_CallDoActionImplementaiton(const Ref(Tuple<ActionParams...>) _paramsToExpand, std::index_sequence   <TuplePackIndex...>)
		{
			                        // ExpandTuplePack<TuplePackIndex>
			DoAction_Implementation(std::get<TuplePackIndex>(_paramsToExpand)...);
		}

		Tuple<ActionParams...> params;

		ActionType action;

		bool done;

	public:   // IAction

		virtual sfn DoAction() -> void override
		{
			ExpandTuple_CallDoActionImplementaiton
			(
				params,
				// MakeTuplePackSequence  <ActionParams...>()
				std::index_sequence_for<ActionParams...>()
			);

			done = true;
		};
	};

	struct ActionPool_Dynamic
	{
		template<typename Type>
		using AllocationsOf = std::forward_list<Type>;

		using TypeIndex         = std::type_index                                    ;
		using Managed_AAction   = SPtr           < IAction                          >;
		using Managed_AActions  = AllocationsOf  < Managed_AAction                  >;
		using AActions_Registry = std::map       <TypeIndex       , Managed_AActions>;

	public:
		template<typename Entry>
		sfn Available(Ref(Entry) _entry) -> bool
		{
			return _entry != aActions_Available.end() ? true : false;
		}

		template<typename Entry>
		sfn Contains(Ref(Entry) _entry) -> bool
		{
			return _entry != aActions_Available.end() ? true : false;
		}

		sfn Make_Managed_Actions() -> Ref(Managed_AActions)
		{
			mAAaction_Allocations.push_front(MakeSPtr<Managed_AActions>());

			return Dref(mAAaction_Allocations.front().get());
		}

		template<typename FunctionType, typename... ActionParams>
		sfn Request_AAction(Delegate< FunctionType> _actionToQueue, ActionParams... _paramsForAction) -> ptr<IAction>
		{
			using ActionType = AAction < FunctionType, ActionParams...>;

			TypeIndex AActionID = typeid(ActionType);

			deduce possibleEntry = aActions_Available.find(AActionID);

			if (Contains(possibleEntry))
			{
				using Element = decltype(possibleEntry->second.begin());

				for (Element possibleAction = possibleEntry->second.begin(); possibleAction != possibleEntry->second.end(); possibleAction++)
				{
					ptr< ActionType> castedEntry = static_cast< ptr< ActionType>>(possibleAction->get());

					if (castedEntry->IsSame(_paramsForAction...))
					{
						return castedEntry;
					}
					else if (castedEntry->Used())
					{
						castedEntry->ReInitalize(_paramsForAction...);

						return castedEntry;
					}
				}

				SPtr< IAction> newAction = MakeSPtr< AAction<FunctionType, ActionParams...>>(_actionToQueue, _paramsForAction...);
				ptr < IAction> returnRef = newAction.get                                                                       ();

				aActions_Available.at(AActionID).push_front(newAction);

				return returnRef;
			}

			SPtr< IAction> newAction = MakeSPtr< AAction<FunctionType, ActionParams...>>(_actionToQueue, _paramsForAction...);
			ptr < IAction> returnRef = newAction.get                                                                       ();

			aActions_Available.insert(std::make_pair(AActionID, Make_Managed_Actions()));

			aActions_Available.at(AActionID).push_front(newAction);

			return returnRef;
		}


	private:
		AllocationsOf< SPtr<Managed_AActions> > mAAaction_Allocations;

		AActions_Registry aActions_Available;
	};

	ActionPool_Dynamic DefaultActionPool_Dynamic;



	struct ActionQueue
	{
		using QueueType = std::deque< ptr<IAction>>;

		

		template<typename FunctionType, typename... ActionParams>
		sfn AddToQueue(Delegate< FunctionType> _actionToQueue, ActionParams... _paramsForAction)
		{
			using GeneratedActionType = AAction<FunctionType, ActionParams...>;

			ptr< IAction > actionRequested = DefaultActionPool_Dynamic.Request_AAction(_actionToQueue, _paramsForAction...);

			if (HasAction())
			{
				bool found = false;

				using Element = decltype(actionQueue.begin());

				for (Element element = actionQueue.begin(); element != actionQueue.end(); element++)
				{
					if ( (*element) == actionRequested )
					{
						found = true;
					}
				}

				if (not found)
				{
					actionQueue.push_front(actionRequested);
				}
			}
			else
			{
				actionQueue.push_front(actionRequested);
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

		sfn HasAction()
		{
			return actionQueue.size() > 0;
		}


		QueueType actionQueue;
	};
}
