/*
Title : Actions
Author: Edward R. Gonzalez

Description: 
This was a little experiment of mine to mess with action binding...

Allows for non-member functions to be binded to an action, implements a functioning queue as well.

TODO: Possibly add support for member functions. Have it so that deduction of delegate typef is not required to add to queue properly (right now it does, see input procedure for example);
*/


#pragma once




#include "Cpp_Alias.hpp"



namespace Actions
{
	using IndexType = DataSize       ;
	using TypeIndex = std::type_index;



	struct IAction
	{
		virtual sfn DoAction  () -> void   = NULL;
	};

	template<typename FunctionType, typename... ActionParams>
	struct AAction : IAction
	{
	public:
		using ActionType = Delegate< FunctionType >;

		AAction(ro Ref(ActionType) _actionToAssign, ro Ref(ActionParams)... _params) :
			action(_actionToAssign),
			params(_params...     ),
			done  (false          )
		{};

		sfn Used() -> bool
		{
			return done;
		}

		sfn IsSame(ro Ref(ActionType) _action, ro Ref(ActionParams)... _paramsForAction) -> bool
		{
			Tuple<ActionParams...> paramsToCheck(_paramsForAction...);

			if (params == paramsToCheck && SameAction(_action))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		sfn SameAction(ro Ref(ActionType) _action)
		{
			if (action.target<FunctionType*>() == _action.target<FunctionType*>())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		sfn ReInitalize(ro Ref(ActionParams)... _params)
		{
			params = Tuple<ActionParams...> (_params...);

			done = false;
		}

	protected:
		virtual sfn DoAction_Implementation(ro Ref(ActionParams)... _params) -> void { action(_params...); }

		template<IndexType... TuplePackIndex>                                                        // TuplePackSequence<TuplePackIndex...>
		sfn ExpandTuple_CallDoActionImplementaiton(ro Ref(Tuple<ActionParams...>) _paramsToExpand, std::index_sequence   <TuplePackIndex...>) -> void
		{
			                        // ExpandTuplePack<TuplePackIndex>
			DoAction_Implementation(std::get<TuplePackIndex>(_paramsToExpand)...);
		}

		Tuple<ActionParams...> params;

		ro Ref(ActionType) action;

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

	// TODO: This doesn't work yet...
	template<typename ObjectType, typename FunctionType, typename... ActionParams>
	class AAction_ObjectBound : public AAction<FunctionType, ActionParams...>
	{
	public:
		using ActionType = Delegate<FunctionType>;

		AAction_ObjectBound(Ref(ObjectType) _objectRef, ro Ref(ActionType) _actionToAssign, ro Ref(ActionParams)... _params) :
			AAction<FunctionType, ActionParams...>::action(_actionToAssign),
			AAction<FunctionType, ActionParams...>::params(_params...     ),
			object                                        (_objectRef     )
		{}


		sfn IsSame(ro Ref(ObjectType) _object, ro Ref(ActionType) _action, ro Ref(ActionParams)... _params) -> bool
		{
			if (SameObject(_object) && SameAction(_action))
			{
				Tuple<ActionParams...> paramsToCheck(_params...);

				if (AAction<FunctionType, ActionParams...>::params == paramsToCheck)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}

		sfn SameObject(ro Ref(ObjectType) _object) -> bool
		{
			if (Address(object) == Address(_object))
			{
				return true;
			}
			else
			{
				return false;
			}
		}


	protected:
		virtual sfn DoAction_Implementation(ro Ref(ActionParams)... _params) -> void override
		{ 
			(Address(object).*AAction<FunctionType, ActionParams...>::action)(_params...);
		}

		template<IndexType... TuplePackIndex>                                                        // TuplePackSequence<TuplePackIndex...>
		sfn ExpandTuple_CallDoActionImplementaiton(ro Ref(Tuple<ActionParams...>) _paramsToExpand, std::index_sequence   <TuplePackIndex...>) -> void
		{
			// ExpandTuplePack<TuplePackIndex>
			DoAction_Implementation(std::get<TuplePackIndex>(_paramsToExpand)...);
		}


		Ref(ObjectType) object;


	public:   // IAction

		virtual sfn DoAction() -> void override
		{
			ExpandTuple_CallDoActionImplementaiton
			(
				AAction<FunctionType, ActionParams...>::params,
				// MakeTuplePackSequence  <ActionParams...>()
				std::index_sequence_for<ActionParams...>()
			);

			AAction<FunctionType, ActionParams...>::params::done = true;
		};
	};

	struct ActionPool_Dynamic
	{
		template<typename Type>
		using AllocationsOf = std::forward_list<Type>;
		
		using Managed_AAction   = SPtr           < IAction                          >;
		using Managed_AActions  = AllocationsOf  < Managed_AAction                  >;
		using AActions_Registry = std::map       <TypeIndex       , Managed_AActions>;

	public:

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
		sfn Request_AAction(ro Ref(Delegate< FunctionType>) _actionToQueue, ro Ref(ActionParams)... _paramsForAction) -> ptr<IAction>
		{
			using ActionType = AAction < FunctionType, ActionParams...>;

			TypeIndex AActionID = typeid(ActionType);

			deduce possibleEntry = aActions_Available.find(AActionID);

			if (Contains(possibleEntry))
			{
				using Element = decltype(possibleEntry->second.begin());

				for (Element possibleAction = possibleEntry->second.begin(); possibleAction != possibleEntry->second.end(); possibleAction++)
				{
					ptr< ActionType> castedEntry = static_cast<ptr< ActionType>>(possibleAction->get());

					if (castedEntry->IsSame(_actionToQueue, _paramsForAction...))
					{
						return castedEntry;
					}
					else if (castedEntry->Used() && castedEntry->SameAction(_actionToQueue))
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

		template<typename ObjectType, typename FunctionType, typename... ActionParams>
		sfn Request_AAction(ro Ref(ObjectType) _objectRef, ro Ref(Delegate< FunctionType>) _actionToQueue, ro Ref(ActionParams)... _paramsForAction) -> ptr<IAction>
		{
			using ActionType = AAction_ObjectBound<ObjectType, FunctionType, ActionParams...>;

			TypeIndex AActionID = typeid(ActionType);

			deduce possibleEntry = aActions_Available.find(AActionID);

			if (Contains(possibleEntry))
			{
				using Element = decltype(possibleEntry->second.begin());

				for (Element possibleAction = possibleEntry->second.begin(); possibleAction != possibleEntry->second.end(); possibleAction++)
				{
					ptr< ActionType> castedEntry = static_cast<ptr< ActionType>>(possibleAction->get());

					if (castedEntry->IsSame(_actionToQueue, _paramsForAction...))
					{
						return castedEntry;
					}
					else if (castedEntry->Used() && castedEntry->SameAction(_actionToQueue))
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

		
	public:
		template<typename FunctionType, typename... ActionParams>
		sfn AddToQueue(ro Ref(Delegate< FunctionType>) _actionToQueue, ro Ref(ActionParams)... _paramsForAction)
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

		template<typename ObjectType, typename FunctionType, typename... ActionParams>
		sfn AddToQueue(ro Ref(ObjectType) _objectRef, ro Ref(Delegate< FunctionType>) _actionToQueue, ro Ref(ActionParams)... _paramsForAction)
		{
			using GeneratedActionType = AAction_ObjectBound<ObjectType, FunctionType, ActionParams...>;

			ptr< IAction > actionRequested = DefaultActionPool_Dynamic.Request_AAction(_objectRef, _actionToQueue, _paramsForAction...);

			if (HasAction())
			{
				bool found = false;

				using Element = decltype(actionQueue.begin());

				for (Element element = actionQueue.begin(); element != actionQueue.end(); element++)
				{
					if ((*element) == actionRequested)
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


	private:

		QueueType actionQueue;
	};
}
