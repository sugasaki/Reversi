//===================================================================================
// Microsoft patterns & practices
// Composite Application Guidance for Windows Presentation Foundation and Silverlight
//===================================================================================
// Copyright (c) Microsoft Corporation.  All rights reserved.
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE.
//===================================================================================
// The example companies, organizations, products, domain names,
// e-mail addresses, logos, people, places, and events depicted
// herein are fictitious.  No association with any real company,
// organization, product, domain name, email address, logo, person,
// places, or events is intended or should be inferred.
//===================================================================================
using System.Collections.Generic;
using MVVM.Infrastructure;

namespace MVVM.Model
{
    #region pseudo generated code - code for domain classes as it could have been generated by a tool

    /// <summary>
    /// The MultipleSelectionQuestion domain object class.
    /// </summary>
    public sealed partial class MultipleSelectionQuestion : Question
    {
        private int? maxSelections;

        private ICollection<string> range;

        private ICollection<string> response;

        /// <summary>
        /// Initializes a new instance of the <see cref="MultipleSelectionQuestion"/> class.
        /// </summary>
        public MultipleSelectionQuestion()
        {
        }

        /// <summary>
        /// Gets or sets the MaxSelections value.
        /// </summary>
        public int? MaxSelections
        {
            get
            {
                return this.maxSelections;
            }

            set
            {
                if (this.maxSelections != value)
                {
                    this.ValidateProperty("MaxSelections", value);
                    this.maxSelections = value;
                    this.RaisePropertyChanged("MaxSelections");
                }
            }
        }

        /// <summary>
        /// Gets or sets the Range value.
        /// </summary>
        public ICollection<string> Range
        {
            get
            {
                return this.range;
            }

            set
            {
                if (this.range != value)
                {
                    this.ValidateProperty("Range", value);
                    this.range = value;
                    this.RaisePropertyChanged("Range");
                }
            }
        }

        /// <summary>
        /// Gets or sets the Response value.
        /// </summary>
        public ICollection<string> Response
        {
            get
            {
                return this.response;
            }

            set
            {
                if (this.response != value)
                {
                    this.ValidateProperty("Response", value);
                    this.response = value;
                    this.RaisePropertyChanged("Response");
                }
            }
        }
    }

    /// <summary>
    /// The NumericQuestion domain object class.
    /// </summary>
    public sealed partial class NumericQuestion : Question
    {
        private int? maxValue;

        private int? response;

        /// <summary>
        /// Initializes a new instance of the <see cref="NumericQuestion"/> class.
        /// </summary>
        public NumericQuestion()
        {
        }

        /// <summary>
        /// Gets or sets the MaxValue value.
        /// </summary>
        public int? MaxValue
        {
            get
            {
                return this.maxValue;
            }

            set
            {
                if (this.maxValue != value)
                {
                    this.ValidateProperty("MaxValue", value);
                    this.maxValue = value;
                    this.RaisePropertyChanged("MaxValue");
                }
            }
        }

        /// <summary>
        /// Gets or sets the Response value.
        /// </summary>
        public int? Response
        {
            get
            {
                return this.response;
            }

            set
            {
                if (this.response != value)
                {
                    this.ValidateProperty("Response", value);
                    this.response = value;
                    this.RaisePropertyChanged("Response");
                }
            }
        }
    }

    /// <summary>
    /// The OpenQuestion domain object class.
    /// </summary>
    public sealed partial class OpenQuestion : Question
    {
        private int maxLength;

        private string response;

        /// <summary>
        /// Initializes a new instance of the <see cref="OpenQuestion"/> class.
        /// </summary>
        public OpenQuestion()
        {
        }

        /// <summary>
        /// Gets or sets the MaxLength value.
        /// </summary>
        public int MaxLength
        {
            get
            {
                return this.maxLength;
            }

            set
            {
                if (this.maxLength != value)
                {
                    this.ValidateProperty("MaxLength", value);
                    this.maxLength = value;
                    this.RaisePropertyChanged("MaxLength");
                }
            }
        }

        /// <summary>
        /// Gets or sets the Response value.
        /// </summary>
        public string Response
        {
            get
            {
                return this.response;
            }

            set
            {
                if (this.response != value)
                {
                    this.ValidateProperty("Response", value);
                    this.response = value;
                    this.RaisePropertyChanged("Response");
                }
            }
        }
    }

    /// <summary>
    /// The Question domain object class.
    /// </summary>
    public abstract partial class Question : DomainObject
    {
        private string questionText;

        /// <summary>
        /// Initializes a new instance of the <see cref="Question"/> class.
        /// </summary>
        protected Question()
        {
        }

        /// <summary>
        /// Gets or sets the QuestionText value.
        /// </summary>
        public string QuestionText
        {
            get
            {
                return this.questionText;
            }

            set
            {
                if (this.questionText != value)
                {
                    this.ValidateProperty("QuestionText", value);
                    this.questionText = value;
                    this.RaisePropertyChanged("QuestionText");
                }
            }
        }
    }

    /// <summary>
    /// The Questionnaire domain object class.
    /// </summary>
    public sealed partial class Questionnaire : DomainObject
    {
        private int? age;

        private string name;

        private int minAge;

        private int? maxAge;

        private int? nameMaxLength;

        private ICollection<Question> questions;

        /// <summary>
        /// Initializes a new instance of the <see cref="Questionnaire"/> class.
        /// </summary>
        public Questionnaire()
        {
        }

        /// <summary>
        /// Gets or sets the Age value.
        /// </summary>
        public int? Age
        {
            get
            {
                return this.age;
            }

            set
            {
                if (this.age != value)
                {
                    this.ValidateProperty("Age", value);
                    this.age = value;
                    this.RaisePropertyChanged("Age");
                }
            }
        }

        /// <summary>
        /// Gets or sets the Name value.
        /// </summary>
        public string Name
        {
            get
            {
                return this.name;
            }

            set
            {
                if (this.name != value)
                {
                    this.ValidateProperty("Name", value);
                    this.name = value;
                    this.RaisePropertyChanged("Name");
                }
            }
        }

        /// <summary>
        /// Gets or sets the MinAge value.
        /// </summary>
        public int MinAge
        {
            get
            {
                return this.minAge;
            }

            set
            {
                if (this.minAge != value)
                {
                    this.ValidateProperty("MinAge", value);
                    this.minAge = value;
                    this.RaisePropertyChanged("MinAge");
                }
            }
        }

        /// <summary>
        /// Gets or sets the MaxAge value.
        /// </summary>
        public int? MaxAge
        {
            get
            {
                return this.maxAge;
            }

            set
            {
                if (this.maxAge != value)
                {
                    this.ValidateProperty("MaxAge", value);
                    this.maxAge = value;
                    this.RaisePropertyChanged("MaxAge");
                }
            }
        }

        /// <summary>
        /// Gets or sets the NameMaxLength value.
        /// </summary>
        public int? NameMaxLength
        {
            get
            {
                return this.nameMaxLength;
            }

            set
            {
                if (this.nameMaxLength != value)
                {
                    this.ValidateProperty("NameMaxLength", value);
                    this.nameMaxLength = value;
                    this.RaisePropertyChanged("NameMaxLength");
                }
            }
        }

        /// <summary>
        /// Gets the collection of associated <see cref="Question"/> entities.
        /// </summary>
        public ICollection<Question> Questions
        {
            get
            {
                if (this.questions == null)
                {
                    this.questions = new List<Question>();
                }

                return this.questions;
            }
        }
    }

    #endregion
}