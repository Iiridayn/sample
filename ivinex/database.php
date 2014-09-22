<?php

/**
 * Let's assume that these values come from a database somewhere. I'll try to
 * name the arrays appropriately... or not. Just going to make my life easier.
 *
 *
 */

	$header = array(
		array('type' => 'image', 'content' => ''),
		array('type' => 'image', 'content' => ''),
		array('type' => 'text', 'content' => <<<EOT
3525 Del Mar Heights Road #413
San Diego, CA  92130
800.560.1469 Voice
858.997.2092 Fax
EOT
		),
	);

	$footer = array(
		array('type' => 'input', 'label' => 'Initials', 'options' => array('css' => 'background-color: yellow')),
		array('type' => 'pagenumber')
	);

	$body = array(
		array('type' => 'section', 'title' => 'Client Information', 'content' => array(
				array('type' => 'input', 'label' => 'Business Name:'),
				array('type' => 'input', 'label' => 'Doctors Name(s):'),
				array('type' => 'input', 'label' => 'Billing Address:'),

				array('type' => 'input', 'label' => 'Business Address:'),

				array('type' => 'input', 'label' => 'Contact Name:'),
				array('type' => 'input', 'label' => 'Business Phone'),
					array('type' => 'input', 'label' => 'Business Fax:'),
				array('type' => 'input', 'label' => 'Contact Email:'),
					array('type' => 'input', 'label' => 'Phone #:'),
				array('type' => 'input', 'label' => 'Website:'),
				array('type' => 'input', 'label' => 'Practice Advisor:'),
					array('type' => 'input', 'label' => 'Date:'),
				array('type' => 'input', 'label' => 'Doctor or Billing Email:'),
			)
		),
		array('type' => 'section', 'title' => 'Products/Services', 'content' => array(
				array('type' => 'table', 'tbody' => array(
						array(
							array('type' => 'checkbox', 'label' => 'Patient Rewards Hub'),
							array('type' => 'checkbox', 'label' => 'Additional Program'),
							array('type' => 'checkbox', 'label' => 'Reward Cards'),
							array('type' => 'checkbox', 'label' => 'Card Readers')
						),
						array(),
						array(
							array(),
							array('type' => 'text', 'content' => 'Total')
						),
						array(
							array('type' => 'multiple', 'content' => array(
								array('type' => 'text', 'content' => 'Patient Rewards Hub'),
								array('type' => 'legal', 'content' => <<<EOT
Electronic Rewards Platform, 1,000 Reward Cards, (2) Card Readers
Marketing Collateral, Training, Unlimited Users
EOT
								)
							),
							array('type' => 'text', 'content' => '$4,980.00'),
							array('type' => 'text', 'content' => '$4,980.00')
						),
						array(
							array('type' => 'multiple', 'content' => array(
								array('type' => 'text', 'content' => 'Additional Reward Program'),
								array('type' => 'checkbox', 'label' => 'Kids Club'),
								array('type' => 'checkbox', 'label' => 'Alumni'),
								array('type' => 'legal', 'content' => <<<EOT
1,000 Reward Cards, (2) Card Readers, Marketing Collateral
EOT
								)
							),
							array('type' => 'text', 'content' => '$1,980.00'),
							array()
						),
						array(
							array('type' => 'text', 'content' => 'Additional Card Reader(s) $125.00 each'),
							array('type' => 'input', 'label' => 'Quantity:'),
							array()
						),
						array(
							array('type' => 'text', 'content' => 'Additional Reward Card(s)'),
							array('type' => 'multiple', 'content' => array(
									array('type' => 'checkbox', 'label' => '500 @ $1.50'),
									array('type' => 'checkbox', 'label' => '1,000 @ $1.25'),
									array('type' => 'checkbox', 'label' => '2,500 @ $1.00'),
									array('type' => 'checkbox', 'label' => '5,000 @ $0.75'),
								)
							),
							array()
						),
						array(
							array('type' => 'text', 'content' => 'Additional Graphic Design'),
							array('type' => 'text', 'content' => '$125.00 hourly'),
							array()
						),
						array(
							array('type' => 'text', 'content' => 'Tax (if applicable)'),
							array(),
							array()
						),
						array(
							array('type' => 'text', 'content' => 'TOTAL'),
							array(),
							array('type' => 'text', 'content' => '$4,980.00')
						),
						array(
							array('type' => 'checkbox', 'label' => 'Online Redemption'),
							array('type' => 'text', 'content' => 'Item Price + $2.00 per item processing fee'),
							array()
						),
						array(
							array('type' => 'multiple', 'content' => array(
									array('type' => 'checkbox', 'label' => 'Monthly Hosting & Database Maintenance'),
									array('type' => 'text', 'content' => '12 month subscription'),
								)
							)
							array('type' => 'text', 'content' => '$129.00 per month'),
							array()
						)
					)
				)
			)
		), // end section
		array('type' => 'legal', 'content' => <<<EOT
The undersigned, on behalf of the client entered above and representing that he/she is duly authorized to bind the Client to this Agreement, agrees to subscribe to the services indicated above and to be bound by the terms and conditions contained herein and in the policies of PracticeGenius, as amended.  The undersigned hereby authorizes PracticeGenius LLC to charge the credit card listed below (or such other card as may be provided to PracticeGenius by the undersigned from time to time for such purpose) the amounts indicated above including charges for redeemed prizes and such other charges as may apply according the terms and conditions of this Agreement.  The undersigned acknowledges that the amount of the charges will depend upon, among other things, the quantity of awards made through the Client’s account and the resulting redemptions and value of the rewards redeemed.  Please read all terms and conditions carefully.
EOT
		), // end type legal
		array('type' => 'section', 'title' => 'Credit Card', 'content' => array(
				array('type' => 'table', 'tbody' => array(
						array(
							array('type' => 'input', 'label' => 'Name of Credit Card Holder:'),
							array('type' => 'input', 'label' => 'Credit Card #:'),
							array('type' => 'input', 'label' => 'Exp. Date:'),
							array('type' => 'input', 'label' => 'Security Code:')
						),
						array(
							array('type' => 'input', 'label' => 'Billing Address on Credit Card:')
						)
						array(
							array('type' => 'input', 'label' => 'Signature:'),
							array('type' => 'input', 'label' => 'Date:')
						)
					)
				)
			) // end content
		), // end section "Credit Card" /* <-- This does create a minor hassle to maintain but I judge it worth it at the time */
		array('type' => 'section', 'title' => 'Terms and Conditions', 'content' => array(
				array('type' => 'legal', 'content' => <<<EOT
IMPORTANT READ CAREFULLY:  These Terms and Conditions and the information indicated on the first page hereof, including without limitation, the services and related fees selected, constitute a legal agreement between the Client (either an individual or a single entity) and PracticeGenius, LLC (“PG”) for use of the Patient Rewards Hub.com, related sites, and related software product and services on PracticeGenius’s network of shared server computers connected to the World-Wide-Web (“Web”), which includes “on-line” or electronic computer software and documentation and may include computer software and associated media and printed materials and other services (collectively, “SOFTWARE PRODUCT” or “SOFTWARE”).  Client is responsible for the acts relating to the SOFTWARE PRODUCT and related services of Client’s employees, agents, officers, directors, and any third party to which Client allows, directly or indirectly, access to the SOFTWARE PRODUCT in complying with this Agreement and otherwise.  By accessing or otherwise using the SOFTWARE PRODUCT, Client agrees to be bound by the terms and conditions of this Agreement.  If you do not agree to the terms of this Agreement, you are not authorized to use the SOFTWARE PRODUCT.  The information and services elected on the first page hereof constitute part of this Agreement.  Additionally, Client agrees to be bound by the terms and conditions of the policies of PracticeGenius, as may be amended from time to time, relating to the services and SOFTWARE PRODUCT including without limitation, privacy policies, and such additional policy terms and conditions are incorporated into and constitute a part of this Agreement.

SOFTWARE PRODUCT LICENSE
The SOFTWARE PRODUCT is protected by copyright laws and international copyright treaties, as well as other intellectual property laws and treaties.  The SOFTWARE PRODUCT is licensed, not sold.  This license entitles you to use the SOFTWARE PRODUCT “Patient Rewards Hub.com” in accordance with the terms and conditions of this Agreement.

You acknowledge that you are required to obtain and provide your own:   +Computer and Operating System  +Internet Connection  +Telephone Line (if required for Internet connection)

1. GRANT OF LICENSE.  This Agreement grants you the following rights:
- Term.  This license shall be continuous, subject to the terms and conditions of this Agreement.
- Access and Use.  The SOFTWARE PRODUCT is intended to be used on-line in conjunction with a hosting service.  You will be provided with a single account per license.  You may access your single account and the SOFTWARE PRODUCT on multiple computers.  You must use the SOFTWARE PRODUCT in compliance with the acceptable use policy published on-line by Patient Rewards Hub.com and with the acceptable use policy of any other applicable network, as they may be amended and updated from time to time.
- Backup copies.  You may make copies of the images, text and your user-content in SOFTWARE PRODUCT for backup and archival purposes.

2. DESCRIPTION OF OTHER RIGHTS AND LIMITATIONS.
- Web-Based Software.  The SOFTWARE PRODUCT represents software which is entirely Web-based.  The SOFTWARE PRODUCT is resident on PracticeGenius’s server computers.  The SOFTWARE PRODUCT is accessed wholly through your own Web browsing software over the Internet.  PracticeGenius will provide you with an account number to use to access the SOFTWARE PRODUCT on the Web.
- Upgrades.  PracticeGenius will provide normal and customary competitive on-line upgrades to the SOFTWARE PRODUCT as determined in the sole discretion of PracticeGenius, without additional charge.  PracticeGenius is under no obligation to make any specific enhancements to or customization of the SOFTWARE PRODUCT. PracticeGenius reserves the right to charge additional amounts for increased functionality, specific enhancements or custom features.
- Connection.  The SOFTWARE PRODUCT is accessed over the Internet but does not include your particular connection to the Internet.  You are responsible for obtaining your own Internet service and connection to the Internet.  Client data will be transmitted over the internet and stored on third party equipment and by using the SOFTWARE PRODUCT, you consent to such transmission and storage.
- Computer and Web Browser.  You will need a computer and operating system capable of running Web browsing software to access the SOFTWARE PRODUCT.  The SOFTWARE PRODUCT does not include a computer, an operating system, or Web browsing software. You may have to update your Web browsing software from time to time in order to obtain the maximum benefit of and access to the SOFTWARE PRODUCT.  It is your own responsibility to obtain, install, configure and update your Web browsing software.
- Limitations on Reverse Engineering, Decompilation, and Disassembly.  You may not reverse engineer, decompile, or disassemble the SOFTWARE PRODUCT, except and only to the extent that such activity is expressly permitted by applicable law notwithstanding this limitation.
- Program violations, fraud or abuse in relation to point or credit or reward usage is subject to appropriate administrative and/or legal action by appropriate governmental authorities.
- Copyright and Trademark Notices.  You must maintain all copyright and trademark notices on all copies of the SOFTWARE PRODUCT.
- No Distribution.  You may not distribute copies of the SOFTWARE PRODUCT to third parties.
- Rental.  Except as provided in the following “Software Transfer” section, you may not rent, lease or lend the SOFTWARE PRODUCT.
- Software Transfer.  You may not transfer your rights under this Agreement unless PracticeGenius agrees in advance in writing and the recipient agrees to the terms of this Agreement.  You may transfer your rights under this Agreement to a third party leasing company that has been approved in advance by PracticeGenius for purposes of leasing the SOFTWARE PRODUCT.
- Termination. PracticeGenius may terminate this Agreement if you fail to comply with its terms and conditions.

3. PAYMENT TERMS.  Client agrees to pay as and when due and authorizes its/his/her credit card to be debited monthly and otherwise as necessary to collect payment for Initial Set Up Fees, License Fees, Data Hosting Fees, monthly charges, prize redemption fees, and all other fees and charges as have been selected on the first page hereof and as otherwise provided herein.  Client acknowledges that the amount of charges the Client incurs will depend upon the rewards that the Client awards to its users and the value of the prizes redeemed by the users as a result of such awards.  Client acknowledges that PracticeGenius reserves the right in its sole discretion to from time to time change its pricing for products and services to be effective upon notice to Client.  PracticeGenius shall not be responsible or held liable for the award of reward points to the Client’s users made by the Client, Client’s agents or employees, or any third party to which Client has allowed access to the SOFTWARE PRODUCT or the charges Client incurs as a result.  The foregoing payment obligation and authorization shall remain in effect until such time that Client notifies PracticeGenius in writing of intentions to terminate this agreement and all payments due including without limitation cancellation fees, have been received by PracticeGenius.  Subscription services shall automatically renew upon the expiration of the initial term for additional 12 month periods unless Client provides PracticeGenius written notice ninety (90) days prior to the expiration of the initial term or any renewal term, as the case may be, of Client’s intent to not renew.  All notices of intent to terminate must be received at least 10 business days prior to Statement issuance date or electronic draft date or termination notice will apply to following month.  Records of deduction(s) will appear on Client’s credit card statement and will serve as a receipt for this transaction.  In the event of an error, client shall notify PracticeGenius within 15 days of receipt of credit card statement and no more than 45 days from actual date of transaction.  A $20 service charge shall automatically be applied in the event that the monthly payment is not honored because of closed accounts, stop payments, or any other reason if PracticeGenius has not received in writing the required 10 business days’ notice of intent to terminate agreement.  A $750 cancellation fee will apply if contract is terminated prior to the expiration of the initial 12 month term. Client will be required to pay all invoices within 20 days from the date of invoice or PracticeGenius and/or its vendor reserves the right to halt services until past due invoices are paid. All invoices and payments will be in U. S. Dollars.  PracticeGenius accepts payments via credit card only (MasterCard and Visa credit cards).  All services and fees are charged on a prepay basis. Payment must be received by PracticeGenius before any product or services are activated. Clients are required to keep a valid debit or credit card on file to be charged for recurring monthly fees.  Clients are responsible for keeping all credit card information current. All changes must be promptly communicated to PracticeGenius and its vendor including changes to credit card type and expiration date, as well as a current e-mail address. All recurring charges are automatically generated and charged to the credit card on file according to this Agreement. Usage of the Program constitutes Client's acceptance of this billing policy.

All accounts more than 30 days past due will be assessed a late fee. Three attempts to contact the Client will be made before any late fee is assessed.  All accounts 60 days past due will be disabled until balances are paid in full. When disabled, all access to the SOFTWARE PRODUCT including Client’s data therein will be suspended and data will be unavailable.  After an account is delinquent 90 days, it will be deactivated. The account record and delinquent balance is then passed on to a third-party collection agency.  PracticeGenius charges a $15.00 late fee for accounts 30 days past due.   If a Client initiates a chargeback, either by mistake or fraudulently, PracticeGenius will assess a $50.00 processing fee for each individual chargeback. Once a chargeback has been received, PracticeGenius will immediately suspend the account until the matter is resolved. If a chargeback against PracticeGenius is performed by a Client to avoid payment (fraudulent chargeback), we will assess the $50.00 fee and add it to the account total. This will then be turned over to a third-party collections service.  In the event an account is submitted to a third-party collections service, a $15.00 processing fee will be assessed to the existing account balance. This fee is in addition to any other fees previously assessed on the account.

4.  OWNERSHIP.  All right, title, and interest in and to the SOFTWARE PRODUCT and any copies thereof and related content which may be accessed through the use of the SOFTWARE PRODUCT, including, without limitation, copyright, trademark and other intellectual property rights, all related services provided hereunder and all concepts, marketing, and inventions, ideas, patent rights, data relating to or resulting from the SOFTWARE PRODUCT and related services (the “Property”) are owned by PracticeGenius or its vendors or other third parties, as the case may be, and such rights, interest, and title may be protected by applicable copyright or other intellectual property laws and treaties.  This Agreement grants you no ownership or other interest in or, other than as expressly provided herein and in related documentation provided by PracticeGenius, any rights to use such Property. Client agrees to sign all necessary documents or take such other actions as PracticeGenius may reasonably request in order to perfect any and all such rights.  Client represents and warrants that it has the legal right to grant the use of any logo, trademark, or other content and property that it may provide to PracticeGenius for use in connection with its account.  This Section 4 shall survive termination of this Agreement.

5.  HOSTING SERVICES.  This Agreement may be made in conjunction with an End-User Hosting Service Agreement, as may be amended from time to time (the “EUHSA”).  You must comply with the terms of any such EUSHA. PracticeGenius reserves the right to discontinue hosting services at any time pursuant to the terms of the EUSHA.

6.  EXPORT RESTRICTIONS.  The SOFTWARE PRODUCT is intended for access only in the U.S. Export of the SOFTWARE PRODUCT from the is regulated under “El controls” of the Export Administration Regulations (EAR, 15 CFR 730-744) of the U.S. Commerce Department, Bureau of Export Administration (BXA). You agree that you will not directly or indirectly, export or re-export the SOFTWARE PRODUCT (or portions thereof) to any country or to any person, entity or end user subject to U.S. export restrictions without first obtaining a Commerce Department export license.  You warrant and represent that neither the BXA nor any other federal agency has suspended, revoked or denied your export privileges.

7.  NOTE ON JAVA SUPPORT.  THE SOFTWARE PRODUCT MAY CONTAIN SUPPORT FOR PROGRAMS WRITTEN IN JAVA.  JAVA TECHNOLOGY IS NOT FAULT TOLERANT AND IS NOT DESIGNED, MANUFACTURED, OR INTENDED FOR USE OR RESALE AS ON-LINE CONTROL EQUIPMENT IN HAZARDOUS ENVIRONMENTS REQUIRING FAIL-SAFE PERFORMANCE, SUCH AS IN THE OPERATION OF NUCLEAR FACILITIES, AIRCRAFT NAVIGATION OR COMMUNICATION SYSTEMS, AIR TRAFFIC CONTROL, DIRECT LIFE SUPPOT MACHINES, OR WEAPONS SYSTEMS, IN WHICH THE FAILURE OF JAVA TECHNOLOGY COULD LEAD DIRECTLY TO DEATH, PERSONAL INJURY, OR SEVERE PHYSICAL OR ENVIRONMENTAL DAMAGE.

8.  WARRANTIES. PracticeGenius expressly disclaims any warranty for the SOFTWARE PRODUCT AND SERVICES PROVIDED HEREUNDER.  The SOFTWARE PRODUCT, SERVICES and any related documentation ARE provided “as is” without any warranty of any kind, either express or implied, including, without limitation, the implied warranties of merchantability, fitness for a particular purpose, or no infringement WHICH ARE EXPRESSLY DISCLAIMED.  The entire risk arising out of use or performance of the SOFTWARE PRODUCT and services remains with you.

9.  NO LIABILITY FOR DAMAGES.  In no event shall PracticeGenius, its suppliers, affiliates, officers, directors, shareholders, agents, employees, subsidiaries, and independent contractors (collectively, “Released parties”) be liable for any damages whatsoever (including, without limitation, damages for loss of business profits, business interruption, loss of business information, errors in information or services, software and hardware, technical problems, or any other pecuniary loss) arising out of the use or inability to use THE PRODUCTS AND SERVICES PROVIDED BY PRACTICEGENIUS, even if PracticeGenius has been advised of the possibility of such damages.  Because some states/jurisdictions do not allow the exclusion or limitation of liability for consequential or incidental damages, the above limitation may not apply to you.  Client hereby indemnifies and holds the Released Parties harmless from any claims or liabilities from its members or from the content, logo or trademarks delivered by Client to PracticeGenius or its vendor for the Program.  This Section 9 shall survive the expiration or termination of this Agreement.

10. GOVERNMENT RESTRICTED RIGHTS AND LEGAL COMPLIANCE.  The SOFTWARE PRODUCT and documentation are provided with RESTRICTED RIGHTS.  Use, duplication, or disclosure by the Government is subject to restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in Technical Data and Computer Software clause at DFARS 252.227-7013 or subparagraphs (c)(1) and (2) of the Commercial Computer Software Restricted Rights at 48 CFR 52.227-19, as applicable.  Manufacturer is PracticeGenius, LLC., San Diego, CA 92130.  Client shall, at its sole cost and expense, obtain all necessary permits and licenses required to participate in the Program, including without limitation, on behalf of its members.  Client shall comply with any and all federal, state, municipal and laws, rules, and regulations applicable to its participation in the Program.  This Program or participation therein is not valid and/or the awarding of reward points and/or the granting of rewards is void where prohibited or restricted by law.  USE OF THE SOFTWARE PRODUCT AND RELATED SERVICES MAY BE SUBJECT TO VARIOUS LAWS AND REGULATIONS DEPENDING ON ITS MANNER OF USE INCLUDING WITHOUT LIMITATION, THE CHILDREN’S ONLINE PRIVACY PROTECTION ACT (“COPPA”) AND THE HEALTH INSURANCE PORTABILITY AND ACCOUNTABILITY ACT (“HIPAA”).  CLIENT AGREES TO USE THE SOFTWARE PRODUCT AND RELATED SERVICES IN A MANNER THAT IS COMPLIANT WITH ALL APPLICABLE LAWS AND REGULATIONS AND AGREES TO INDEMNIFY AND HOLD HARMLESS PRACTICEGENIUS FOR ANY CLAIMS OR LOSSES INCURRED BY PRACTICEGENIUS AS A RESULT OF NON-COMPLIANCE BY CLIENT OR CLIENT’S AGENTS, EMPLOYEES, OFFICERS, DIRECTORS OR ANY third party to which Client allows, directly or indirectly, access to the SOFTWARE PRODUCT.

11. CONFIDENTIALITY AND NON-DISCLOSURE TERMS. (a) Each Party (the “Receiving Party”) shall preserve the confidentiality of (i) all information not generally known outside the disclosing party (the “Disclosing Party”) disclosed to it by the Disclosing Party and (ii) any information developed by Receiving Party based upon such information (collectively, “Confidential Information”). Confidential Information shall not include information which: (w) has become publicly known to the Receiving Party without breach of this Agreement or any other confidentiality obligation; (x) has been given to the Receiving Party by a third party with a legal right to so disclose; (y) was known to the Receiving Party at the time of disclosure as evidenced by its written records; or (z) was independently developed by the Receiving Party without reference to or use of the Confidential Information. Confidential information shall include without limitation Client pricing and pricing discounts, promotions, and any other pricing deviations (collectively, “Pricing”).  Client agrees that all Pricing and all [documents, content, and other work product provided to Client by PracticeGenius] pursuant to the terms of this Agreement are for the exclusive use of the Client and may not be disclosed to any other person or entity without the prior written consent of PracticeGenius. (b) Client acknowledges that PracticeGenius will suffer immediate and irreparable harm in the event of actual or threatened breach by Client of the obligations of non-disclosure assumed hereunder and PracticeGenius shall be entitled to seek immediate injunctive relief restraining the party from the breach or threatened breach, in addition to any other remedies available to it in law or equity. Client shall be responsible for a breach of this provision by its employees and agents and represents to PracticeGenius that its employees have undertaken general obligations of nondisclosure with regard to proprietary and confidential information to which they have access during the course of their employment with Client no less restrictive than those imposed on Client hereunder.

12. MISCELLANEOUS.  If any provision of this Agreement is found to be invalid or unenforceable by a court of competent jurisdiction, such provision shall be severed from the remainder of this Agreement, which will otherwise remain in full force and effect.   The rights and obligations of Client under the Program and this Agreement may not be assigned or transferred without PracticeGenius’ prior written consent.  This Agreement shall be construed in accordance with Nevada law without regard to the conflict of laws rules or principles thereof. It is agreed that both parties hereby submit to the jurisdiction of Nevada state and federal courts.  Both parties hereby submit to the jurisdiction of the federal and state courts of Nevada.  Facsimile signatures to this Agreement shall be accepted as original signatures. PracticeGenius’ vendor is a third party beneficiary to this Agreement. If the performance or observance of this Agreement or of any obligation herein is prevented or delayed by reason of an act of God, civil commotion, storm, fire, riots, strikes, legal moratorium, war, revolution, or action by government, the party so affected shall, upon prompt notice of such cause being given to the other party, be excused from such performance or observance, only to the extent of such prevention or during the period of such delay, provided that the party so affected shall use its best efforts to avoid or remove the cause(s) of non‑performance and observance with utmost dispatch; provided however, that the foregoing shall not excuse non-payment.  Client agrees that in its relationship with PracticeGenius under this Agreement it is acting in the capacity of an independent contractor and that it has no authority to represent or act on behalf of PracticeGenius without PracticeGenius’ prior written consent. Any notice required or desired to be given regarding this Agreement shall be in writing and may be given by personal delivery, by facsimile transmission, by overnight courier service, or by certified mail.  A notice shall be deemed to have been given (i) on the third (3rd) business day after mailing if such notice was deposited in the United States mail, certified, postage prepaid, addressed to the party to be served at the most recent address provided by the recipient for business correspondence (the “Address for Notices”), (ii) when delivered if given by personal delivery to the party at its Address for Notice, (iii) the same day when sent by facsimile or electronic email transmission during normal business hours, receipt confirmed by transmission equipment, and (iv) in all other cases when actually received at the party’s Address for Notices.  Sections 4, 8, 9, and 11 shall survive the expiration or termination of this Agreement
EOT;
				) //end type legal
			) // end content
		) // end section
	); // end $body data

?>