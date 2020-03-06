class ExpressionOperators
{
    public:
		static bool EvaluateBinary(ExpressionTokenizer* pExpressionTokenizer, int index)
		{
			ExpressionNode* pNode = pExpressionTokenizer->GetNode(index);

			int leftOperandIndex = index - 1;
			while (pExpressionTokenizer->GetNode(leftOperandIndex)->IsEmpty())
			{
				leftOperandIndex--;
			}

			int rightOperandIndex = index + 1;
			while (pExpressionTokenizer->GetNode(rightOperandIndex)->IsEmpty())
			{
				rightOperandIndex++;
			}

			ExpressionNode* pOperandLeft = pExpressionTokenizer->GetNode(leftOperandIndex);
			ExpressionNode* pOperandRight = pExpressionTokenizer->GetNode(rightOperandIndex);

			float valueLeft = pOperandLeft->_value.GetValueFloat(0);
			float valueRight = pOperandRight->_value.GetValueFloat(0);

			switch (*pNode->_pItem)
			{
			case '*':
				pNode->_value = valueLeft * valueRight;
				break;

			case '/':
				pNode->_value = valueLeft / valueRight;
				break;

			case '%':
				pNode->_value = (int)valueLeft % (int)valueRight;
				break;

			case '+':
				pNode->_value = valueLeft + valueRight;
				break;

			case '-':
				pNode->_value = valueLeft - valueRight;
				break;

			case '>':
				if (*(pNode->_pItem + 1) == '=')
				{
					pNode->_value = valueLeft >= valueRight;
				}
				else
				{
					pNode->_value = valueLeft > valueRight;
				}
				break;

			case '<':
				if (*(pNode->_pItem + 1) == '=')
				{
					pNode->_value = valueLeft <= valueRight;
				}
				else
				{
					pNode->_value = valueLeft < valueRight;
				}
				break;

			case '!':
				if (*(pNode->_pItem + 1) == '=')
				{
					pNode->_value = valueLeft != valueRight;
				}
				break;

			case '=':
				if (*(pNode->_pItem + 1) == '=')
				{
					pNode->_value = valueLeft == valueRight;
				}
				break;

			default:
				// handle not-matched here with direct return
				return false;
			}

			pNode->_pItem = 0;
			pExpressionTokenizer->SetNodeEmpty(leftOperandIndex);
			pExpressionTokenizer->SetNodeEmpty(rightOperandIndex);

			return true;
		}

		static void EvaluateUnaryMatches(ExpressionTokenizer* pExpressionTokenizer, int start, int max)
		{
			const char *pMatchesTemplate = "+-!";

			for (int i = start; i <= max; i++)
			{
				ExpressionNode* pNode = pExpressionTokenizer->GetNode(i);

				if (pNode->_pItem != 0)
				{
					const char *pMatches = pMatchesTemplate;

					while (*pMatches != '\0')
					{
						if (pNode->StartsWith(*pMatches))
						{
							// Unaries are matched by having a non-value (start or operator) on the left and a value on the right.

							if (i == start || pExpressionTokenizer->GetNode(i - 1)->_value.IsNan())
							{
								ExpressionNode* pNext = pExpressionTokenizer->GetNode(i + 1);
								if (!pNext->_value.IsNan())
								{
									Variable newValue;
									switch (*pMatches)
									{
									case '-':
										newValue = -pNext->_value.GetValueFloat(0);
										break;

									case '+':
										newValue = pNext->_value.GetValueFloat(0);
										break;

									case '!':
										newValue = !pNext->_value.GetValueFloat(0);
										break;

									}
									pNext->_value.SetValue(0, newValue.GetValueFloat(0));
									pExpressionTokenizer->SetNodeEmpty(i);
								}
							}
							break;
						}

						pMatches++;
					}
				}
			}
		}

		static void EvaluateBinaryMatches(ExpressionTokenizer* pExpressionTokenizer, int start, int max, const char *pMatchesTemplate)
		{
			for (int i = start; i <= max; i++)
			{
				ExpressionNode* pNode = pExpressionTokenizer->GetNode(i);

				if (pNode->_pItem != 0)
				{
					const char *pMatches = pMatchesTemplate;

					while (*pMatches != '\0')
					{
						if (pNode->StartsWith(*pMatches))
						{
							if (ExpressionOperators::EvaluateBinary(pExpressionTokenizer, i))
							{
								i -= 2;
							}
							break;
						}

						pMatches++;
					}
				}
			}
		}
};